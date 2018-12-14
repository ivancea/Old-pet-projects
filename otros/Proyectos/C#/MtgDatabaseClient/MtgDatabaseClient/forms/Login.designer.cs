using System.ComponentModel;
using System.Windows.Forms;

namespace MtgDatabaseClient.forms
{
    partial class Login
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Login));
            this.textServer = new System.Windows.Forms.TextBox();
            this.textUserid = new System.Windows.Forms.TextBox();
            this.textPassword = new System.Windows.Forms.TextBox();
            this.textDatabase = new System.Windows.Forms.TextBox();
            this.labelServer = new System.Windows.Forms.Label();
            this.labelUserid = new System.Windows.Forms.Label();
            this.labelPassword = new System.Windows.Forms.Label();
            this.labelDatabase = new System.Windows.Forms.Label();
            this.buttonAccess = new System.Windows.Forms.Button();
            this.labelError = new System.Windows.Forms.Label();
            this.buttonBlock = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textServer
            // 
            this.textServer.Enabled = false;
            this.textServer.Location = new System.Drawing.Point(150, 20);
            this.textServer.Name = "textServer";
            this.textServer.Size = new System.Drawing.Size(100, 20);
            this.textServer.TabIndex = 0;
            // 
            // textUserid
            // 
            this.textUserid.Enabled = false;
            this.textUserid.Location = new System.Drawing.Point(150, 120);
            this.textUserid.Name = "textUserid";
            this.textUserid.Size = new System.Drawing.Size(100, 20);
            this.textUserid.TabIndex = 1;
            // 
            // textPassword
            // 
            this.textPassword.Location = new System.Drawing.Point(150, 160);
            this.textPassword.Name = "textPassword";
            this.textPassword.PasswordChar = '●';
            this.textPassword.Size = new System.Drawing.Size(100, 20);
            this.textPassword.TabIndex = 2;
            // 
            // textDatabase
            // 
            this.textDatabase.Enabled = false;
            this.textDatabase.Location = new System.Drawing.Point(150, 60);
            this.textDatabase.Name = "textDatabase";
            this.textDatabase.Size = new System.Drawing.Size(100, 20);
            this.textDatabase.TabIndex = 3;
            // 
            // labelServer
            // 
            this.labelServer.AutoSize = true;
            this.labelServer.Location = new System.Drawing.Point(88, 23);
            this.labelServer.Name = "labelServer";
            this.labelServer.Size = new System.Drawing.Size(46, 13);
            this.labelServer.TabIndex = 4;
            this.labelServer.Text = "Servidor";
            // 
            // labelUserid
            // 
            this.labelUserid.AutoSize = true;
            this.labelUserid.Location = new System.Drawing.Point(91, 123);
            this.labelUserid.Name = "labelUserid";
            this.labelUserid.Size = new System.Drawing.Size(43, 13);
            this.labelUserid.TabIndex = 5;
            this.labelUserid.Text = "Usuario";
            // 
            // labelPassword
            // 
            this.labelPassword.AutoSize = true;
            this.labelPassword.Location = new System.Drawing.Point(73, 163);
            this.labelPassword.Name = "labelPassword";
            this.labelPassword.Size = new System.Drawing.Size(61, 13);
            this.labelPassword.TabIndex = 6;
            this.labelPassword.Text = "Contraseña";
            // 
            // labelDatabase
            // 
            this.labelDatabase.AutoSize = true;
            this.labelDatabase.Location = new System.Drawing.Point(59, 63);
            this.labelDatabase.Name = "labelDatabase";
            this.labelDatabase.Size = new System.Drawing.Size(75, 13);
            this.labelDatabase.TabIndex = 7;
            this.labelDatabase.Text = "Base de datos";
            // 
            // buttonAccess
            // 
            this.buttonAccess.Location = new System.Drawing.Point(117, 200);
            this.buttonAccess.Name = "buttonAccess";
            this.buttonAccess.Size = new System.Drawing.Size(75, 23);
            this.buttonAccess.TabIndex = 8;
            this.buttonAccess.Text = "Acceder";
            this.buttonAccess.UseVisualStyleBackColor = true;
            this.buttonAccess.Click += new System.EventHandler(this.buttonAccess_Click);
            // 
            // labelError
            // 
            this.labelError.ForeColor = System.Drawing.Color.Red;
            this.labelError.Location = new System.Drawing.Point(62, 238);
            this.labelError.Name = "labelError";
            this.labelError.Size = new System.Drawing.Size(188, 13);
            this.labelError.TabIndex = 9;
            this.labelError.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // buttonBlock
            // 
            this.buttonBlock.Location = new System.Drawing.Point(198, 200);
            this.buttonBlock.Name = "buttonBlock";
            this.buttonBlock.Size = new System.Drawing.Size(75, 23);
            this.buttonBlock.TabIndex = 10;
            this.buttonBlock.Text = "Desbloquear";
            this.buttonBlock.UseVisualStyleBackColor = true;
            this.buttonBlock.Click += new System.EventHandler(this.buttonBlock_Click);
            // 
            // Login
            // 
            this.AcceptButton = this.buttonAccess;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(304, 271);
            this.Controls.Add(this.buttonBlock);
            this.Controls.Add(this.labelError);
            this.Controls.Add(this.buttonAccess);
            this.Controls.Add(this.labelDatabase);
            this.Controls.Add(this.labelPassword);
            this.Controls.Add(this.labelUserid);
            this.Controls.Add(this.labelServer);
            this.Controls.Add(this.textDatabase);
            this.Controls.Add(this.textPassword);
            this.Controls.Add(this.textUserid);
            this.Controls.Add(this.textServer);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.Name = "Login";
            this.Text = "Login";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private TextBox textServer;
        private TextBox textUserid;
        private TextBox textPassword;
        private TextBox textDatabase;
        private Label labelServer;
        private Label labelUserid;
        private Label labelPassword;
        private Label labelDatabase;
        private Button buttonAccess;
        private Label labelError;
        private Button buttonBlock;

    }
}