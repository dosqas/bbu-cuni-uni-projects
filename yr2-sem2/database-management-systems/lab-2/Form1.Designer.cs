namespace LAB2
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            tableLayoutPanel1 = new TableLayoutPanel();
            tableLayoutPanel2 = new TableLayoutPanel();
            MasterDataGridView = new DataGridView();
            DetailDataGridView = new DataGridView();
            button1 = new Button();
            tableLayoutPanel3 = new TableLayoutPanel();
            MasterTableNameLabel = new Label();
            DetailTableNameLabel = new Label();
            tableLayoutPanel1.SuspendLayout();
            tableLayoutPanel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)MasterDataGridView).BeginInit();
            ((System.ComponentModel.ISupportInitialize)DetailDataGridView).BeginInit();
            tableLayoutPanel3.SuspendLayout();
            SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            tableLayoutPanel1.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            tableLayoutPanel1.BackColor = Color.Transparent;
            tableLayoutPanel1.ColumnCount = 1;
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tableLayoutPanel1.Controls.Add(tableLayoutPanel2, 0, 1);
            tableLayoutPanel1.Controls.Add(button1, 0, 2);
            tableLayoutPanel1.Controls.Add(tableLayoutPanel3, 0, 0);
            tableLayoutPanel1.Location = new Point(3, 1);
            tableLayoutPanel1.Margin = new Padding(3, 4, 3, 4);
            tableLayoutPanel1.Name = "tableLayoutPanel1";
            tableLayoutPanel1.RowCount = 3;
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 21.5053768F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 78.49462F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Absolute, 88F));
            tableLayoutPanel1.Size = new Size(912, 599);
            tableLayoutPanel1.TabIndex = 5;
            // 
            // tableLayoutPanel2
            // 
            tableLayoutPanel2.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            tableLayoutPanel2.ColumnCount = 2;
            tableLayoutPanel2.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tableLayoutPanel2.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tableLayoutPanel2.Controls.Add(MasterDataGridView, 0, 0);
            tableLayoutPanel2.Controls.Add(DetailDataGridView, 1, 0);
            tableLayoutPanel2.Location = new Point(3, 113);
            tableLayoutPanel2.Margin = new Padding(3, 4, 3, 4);
            tableLayoutPanel2.Name = "tableLayoutPanel2";
            tableLayoutPanel2.RowCount = 1;
            tableLayoutPanel2.RowStyles.Add(new RowStyle(SizeType.Percent, 50F));
            tableLayoutPanel2.Size = new Size(906, 393);
            tableLayoutPanel2.TabIndex = 0;
            // 
            // MasterDataGridView
            // 
            MasterDataGridView.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            MasterDataGridView.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            MasterDataGridView.Location = new Point(3, 4);
            MasterDataGridView.Margin = new Padding(3, 4, 3, 4);
            MasterDataGridView.Name = "MasterDataGridView";
            MasterDataGridView.RowHeadersWidth = 51;
            MasterDataGridView.Size = new Size(447, 385);
            MasterDataGridView.TabIndex = 0;
            // 
            // DetailDataGridView
            // 
            DetailDataGridView.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            DetailDataGridView.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            DetailDataGridView.Location = new Point(456, 4);
            DetailDataGridView.Margin = new Padding(3, 4, 3, 4);
            DetailDataGridView.Name = "DetailDataGridView";
            DetailDataGridView.RowHeadersWidth = 51;
            DetailDataGridView.Size = new Size(447, 385);
            DetailDataGridView.TabIndex = 1;
            // 
            // button1
            // 
            button1.Anchor = AnchorStyles.None;
            button1.Font = new Font("Segoe UI", 13.8F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button1.Location = new Point(372, 530);
            button1.Margin = new Padding(3, 4, 3, 4);
            button1.Name = "button1";
            button1.Size = new Size(167, 49);
            button1.TabIndex = 1;
            button1.Text = "Update";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // tableLayoutPanel3
            // 
            tableLayoutPanel3.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            tableLayoutPanel3.ColumnCount = 2;
            tableLayoutPanel3.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tableLayoutPanel3.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tableLayoutPanel3.Controls.Add(MasterTableNameLabel, 0, 0);
            tableLayoutPanel3.Controls.Add(DetailTableNameLabel, 1, 0);
            tableLayoutPanel3.Location = new Point(3, 4);
            tableLayoutPanel3.Margin = new Padding(3, 4, 3, 4);
            tableLayoutPanel3.Name = "tableLayoutPanel3";
            tableLayoutPanel3.RowCount = 1;
            tableLayoutPanel3.RowStyles.Add(new RowStyle(SizeType.Percent, 50F));
            tableLayoutPanel3.Size = new Size(906, 101);
            tableLayoutPanel3.TabIndex = 2;
            // 
            // MasterTableNameLabel
            // 
            MasterTableNameLabel.Anchor = AnchorStyles.None;
            MasterTableNameLabel.AutoSize = true;
            MasterTableNameLabel.Font = new Font("Segoe UI", 18F, FontStyle.Regular, GraphicsUnit.Point, 0);
            MasterTableNameLabel.Location = new Point(183, 30);
            MasterTableNameLabel.Name = "MasterTableNameLabel";
            MasterTableNameLabel.Size = new Size(86, 41);
            MasterTableNameLabel.TabIndex = 0;
            MasterTableNameLabel.Text = "Films";
            // 
            // DetailTableNameLabel
            // 
            DetailTableNameLabel.Anchor = AnchorStyles.None;
            DetailTableNameLabel.AutoSize = true;
            DetailTableNameLabel.Font = new Font("Segoe UI", 18F, FontStyle.Regular, GraphicsUnit.Point, 0);
            DetailTableNameLabel.Location = new Point(607, 30);
            DetailTableNameLabel.Name = "DetailTableNameLabel";
            DetailTableNameLabel.Size = new Size(145, 41);
            DetailTableNameLabel.TabIndex = 1;
            DetailTableNameLabel.Text = "Programs";
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(914, 600);
            Controls.Add(tableLayoutPanel1);
            Margin = new Padding(3, 4, 3, 4);
            Name = "Form1";
            Text = "Form1";
            tableLayoutPanel1.ResumeLayout(false);
            tableLayoutPanel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)MasterDataGridView).EndInit();
            ((System.ComponentModel.ISupportInitialize)DetailDataGridView).EndInit();
            tableLayoutPanel3.ResumeLayout(false);
            tableLayoutPanel3.PerformLayout();
            ResumeLayout(false);
        }

        #endregion

        private TableLayoutPanel tableLayoutPanel1;
        private TableLayoutPanel tableLayoutPanel2;
        private Button button1;
        private TableLayoutPanel tableLayoutPanel3;
        private Label MasterTableNameLabel;
        private Label DetailTableNameLabel;
        private DataGridView MasterDataGridView;
        private DataGridView DetailDataGridView;
    }
}
