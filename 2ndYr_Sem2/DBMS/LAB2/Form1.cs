using Microsoft.Data.SqlClient;
using System.Data;

namespace dbmsWF1
{
    public partial class Form1 : Form
    {
        SqlConnection conn;
        SqlDataAdapter daProgram;
        SqlDataAdapter daFilm;
        DataSet dSet;
        BindingSource bsProgram;
        BindingSource bsFilm;

        SqlCommandBuilder cmdBuilder;

        string queryProgram;
        string queryFilm;

        public Form1()
        {
            InitializeComponent();
            FillData();
        }

        private void FillData()
        {
            try
            {
                conn = new SqlConnection(getConnectionString());
            }
            catch (SystemException)
            {
                Console.WriteLine("Error on initiating database connection");
            }

            queryProgram = "SELECT * FROM Program";
            queryFilm = "SELECT * FROM Film";

            daProgram = new SqlDataAdapter(queryProgram, conn);
            daFilm = new SqlDataAdapter(queryFilm, conn);

            dSet = new DataSet();

            try
            {
                daProgram.Fill(dSet, "Program");
                daFilm.Fill(dSet, "Film");
            }
            catch (SystemException)
            {
                Console.WriteLine("Error filling the DataSets.");
            }

            cmdBuilder = new SqlCommandBuilder(daProgram);

            dSet.Relations.Add("FilmProgram",
                dSet.Tables["Film"]!.Columns["IDFilm"]!,
                dSet.Tables["Program"]!.Columns["IDFilm"]!);

            bsFilm = new BindingSource();
            bsFilm.DataSource = dSet.Tables["Film"];
            bsProgram = new BindingSource(bsFilm, "FilmProgram");

            this.dataGridView1.DataSource = bsFilm;
            this.dataGridView2.DataSource = bsProgram;

            cmdBuilder.GetUpdateCommand();
        }

        private string getConnectionString()
        {
            return "Server=localhost\\SQLEXPRESS;Database=CinemaDB;Trusted_Connection=True;TrustServerCertificate=True;";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                daProgram.Update(dSet, "Program");
            }
            catch (ArgumentNullException)
            {
                Console.WriteLine("Error on updating the database: DataSet is null");
            }
            catch (InvalidOperationException)
            {
                Console.WriteLine("Error on updating the database: Update operation is invalid");
            }
            catch (DBConcurrencyException)
            {
                Console.WriteLine("Error on updating the database: Concurrency exception occured");
            }
        }
    }
}
