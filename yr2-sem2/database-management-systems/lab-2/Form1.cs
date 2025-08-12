using Microsoft.Data.SqlClient;
using Newtonsoft.Json;
using System.Data;
using System.Configuration;

namespace LAB2
{
    public partial class Form1 : Form
    {
        SqlConnection? _conn;
        SqlDataAdapter? _daMaster;
        SqlDataAdapter? _daDetail;
        DataSet? _dSet;
        BindingSource? _bsMaster;
        BindingSource? _bsDetail;

        SqlCommandBuilder? _cmdBuilder;

        Config? _config;

        public Form1()
        {
            InitializeComponent();
            string configName = ConfigurationManager.AppSettings["CurrentConfigName"]!;
            FillData(configName!);
        }

        private Config? LoadConfig(string configName)
        {
            string configContent;
            try
            {
                configContent = File.ReadAllText("config.json");
                var configs = JsonConvert.DeserializeObject<List<Config>>(configContent);
                return configs!.FirstOrDefault(c => c.ConfigName == configName);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failure reading the config file: {ex.Message}");
                Environment.Exit(-1);
            }

            return null;
        }

        private void FillData(string configName)
        {
            _config = LoadConfig(configName);
            if (_config == null)
            {
                Console.WriteLine("Configuration not found or failed to load.");
                Environment.Exit(-1);
            }

            try
            {
                _conn = new SqlConnection(ConfigurationManager.ConnectionStrings["CinemaDB"].ConnectionString);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error on initiating database connection: {ex.Message}");
                Environment.Exit(-1);
            }

            _daMaster = new SqlDataAdapter(_config.MasterQuery, _conn);
            _daDetail = new SqlDataAdapter(_config.DetailQuery, _conn);

            _dSet = new DataSet();

            try
            {
                _daMaster.Fill(_dSet, _config.MasterTableName!);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error filling the Master DataSet: {ex.Message}");
                Environment.Exit(-1);
            }

            try
            {
                _daDetail.Fill(_dSet, _config.DetailTableName!);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error filling the Detail DataSet: {ex.Message}");
                Environment.Exit(-1);
            }

            _cmdBuilder = new SqlCommandBuilder(_daDetail);

            try
            {
                _dSet.Relations.Add(_config.ConfigName,
                    _dSet.Tables[_config.MasterTableName]!.Columns[_config.Relation!.MasterColumnName!]!,
                    _dSet.Tables[_config.DetailTableName]!.Columns[_config.Relation.DetailColumnName!]!);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error adding the master-detail relation to the DataSet: {ex.Message}");
                Environment.Exit(-1);
            }

            _bsMaster = new BindingSource();
            _bsMaster.DataSource = _dSet.Tables[_config.MasterTableName];
            _bsDetail = new BindingSource(_bsMaster, _config.ConfigName);

            this.MasterDataGridView.DataSource = _bsMaster;
            this.MasterTableNameLabel.Text = _config.MasterTableName;

            this.DetailDataGridView.DataSource = _bsDetail;
            this.DetailTableNameLabel.Text = _config.DetailTableName;

            _cmdBuilder.GetUpdateCommand();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                _daDetail!.Update(_dSet!, _config!.DetailTableName!);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error on updating the Detail database: {ex.Message}");
            }
        }
    }
}
