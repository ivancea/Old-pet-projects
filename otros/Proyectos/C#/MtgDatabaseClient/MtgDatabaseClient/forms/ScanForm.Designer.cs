using System.ComponentModel;

namespace MtgDatabaseClient.forms {
    partial class ScanForm {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ScanForm));
            this.buttonStartScanning = new System.Windows.Forms.Button();
            this.labelInitialPageInfo = new System.Windows.Forms.Label();
            this.textInitialPage = new System.Windows.Forms.TextBox();
            this.labelActualName = new System.Windows.Forms.Label();
            this.labelActualPage = new System.Windows.Forms.Label();
            this.labelActualPageInfo = new System.Windows.Forms.Label();
            this.labelActualIdInfo = new System.Windows.Forms.Label();
            this.labelActualId = new System.Windows.Forms.Label();
            this.labelActualType = new System.Windows.Forms.Label();
            this.labelActualTypeInfo = new System.Windows.Forms.Label();
            this.labelActualNameInfo = new System.Windows.Forms.Label();
            this.labelTotalScannedInfo = new System.Windows.Forms.Label();
            this.labelTotalScanned = new System.Windows.Forms.Label();
            this.labelActualRarityInfo = new System.Windows.Forms.Label();
            this.labelActualRarity = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // buttonStartScanning
            // 
            this.buttonStartScanning.Location = new System.Drawing.Point(15, 58);
            this.buttonStartScanning.Name = "buttonStartScanning";
            this.buttonStartScanning.Size = new System.Drawing.Size(100, 23);
            this.buttonStartScanning.TabIndex = 0;
            this.buttonStartScanning.Text = "Start scanning";
            this.buttonStartScanning.UseVisualStyleBackColor = true;
            this.buttonStartScanning.Click += new System.EventHandler(this.buttonStartScanning_Click);
            // 
            // labelInitialPageInfo
            // 
            this.labelInitialPageInfo.AutoSize = true;
            this.labelInitialPageInfo.Location = new System.Drawing.Point(12, 22);
            this.labelInitialPageInfo.Name = "labelInitialPageInfo";
            this.labelInitialPageInfo.Size = new System.Drawing.Size(61, 13);
            this.labelInitialPageInfo.TabIndex = 1;
            this.labelInitialPageInfo.Text = "Initial page:";
            // 
            // textInitialPage
            // 
            this.textInitialPage.Location = new System.Drawing.Point(76, 19);
            this.textInitialPage.Name = "textInitialPage";
            this.textInitialPage.Size = new System.Drawing.Size(100, 20);
            this.textInitialPage.TabIndex = 2;
            // 
            // labelActualName
            // 
            this.labelActualName.AutoSize = true;
            this.labelActualName.Location = new System.Drawing.Point(77, 154);
            this.labelActualName.Name = "labelActualName";
            this.labelActualName.Size = new System.Drawing.Size(10, 13);
            this.labelActualName.TabIndex = 3;
            this.labelActualName.Text = "-";
            // 
            // labelActualPage
            // 
            this.labelActualPage.AutoSize = true;
            this.labelActualPage.Location = new System.Drawing.Point(76, 103);
            this.labelActualPage.Name = "labelActualPage";
            this.labelActualPage.Size = new System.Drawing.Size(10, 13);
            this.labelActualPage.TabIndex = 4;
            this.labelActualPage.Text = "-";
            // 
            // labelActualPageInfo
            // 
            this.labelActualPageInfo.AutoSize = true;
            this.labelActualPageInfo.Location = new System.Drawing.Point(8, 103);
            this.labelActualPageInfo.Name = "labelActualPageInfo";
            this.labelActualPageInfo.Size = new System.Drawing.Size(65, 13);
            this.labelActualPageInfo.TabIndex = 5;
            this.labelActualPageInfo.Text = "ActualPage:";
            // 
            // labelActualIdInfo
            // 
            this.labelActualIdInfo.AutoSize = true;
            this.labelActualIdInfo.Location = new System.Drawing.Point(52, 129);
            this.labelActualIdInfo.Name = "labelActualIdInfo";
            this.labelActualIdInfo.Size = new System.Drawing.Size(19, 13);
            this.labelActualIdInfo.TabIndex = 6;
            this.labelActualIdInfo.Text = "Id:";
            // 
            // labelActualId
            // 
            this.labelActualId.AutoSize = true;
            this.labelActualId.Location = new System.Drawing.Point(77, 129);
            this.labelActualId.Name = "labelActualId";
            this.labelActualId.Size = new System.Drawing.Size(10, 13);
            this.labelActualId.TabIndex = 7;
            this.labelActualId.Text = "-";
            // 
            // labelActualType
            // 
            this.labelActualType.AutoSize = true;
            this.labelActualType.Location = new System.Drawing.Point(77, 178);
            this.labelActualType.Name = "labelActualType";
            this.labelActualType.Size = new System.Drawing.Size(10, 13);
            this.labelActualType.TabIndex = 8;
            this.labelActualType.Text = "-";
            // 
            // labelActualTypeInfo
            // 
            this.labelActualTypeInfo.AutoSize = true;
            this.labelActualTypeInfo.Location = new System.Drawing.Point(37, 178);
            this.labelActualTypeInfo.Name = "labelActualTypeInfo";
            this.labelActualTypeInfo.Size = new System.Drawing.Size(34, 13);
            this.labelActualTypeInfo.TabIndex = 9;
            this.labelActualTypeInfo.Text = "Type:";
            // 
            // labelActualNameInfo
            // 
            this.labelActualNameInfo.AutoSize = true;
            this.labelActualNameInfo.Location = new System.Drawing.Point(33, 154);
            this.labelActualNameInfo.Name = "labelActualNameInfo";
            this.labelActualNameInfo.Size = new System.Drawing.Size(38, 13);
            this.labelActualNameInfo.TabIndex = 10;
            this.labelActualNameInfo.Text = "Name:";
            // 
            // labelTotalScannedInfo
            // 
            this.labelTotalScannedInfo.AutoSize = true;
            this.labelTotalScannedInfo.Location = new System.Drawing.Point(137, 63);
            this.labelTotalScannedInfo.Name = "labelTotalScannedInfo";
            this.labelTotalScannedInfo.Size = new System.Drawing.Size(78, 13);
            this.labelTotalScannedInfo.TabIndex = 13;
            this.labelTotalScannedInfo.Text = "Total scanned:";
            // 
            // labelTotalScanned
            // 
            this.labelTotalScanned.AutoSize = true;
            this.labelTotalScanned.Location = new System.Drawing.Point(221, 63);
            this.labelTotalScanned.Name = "labelTotalScanned";
            this.labelTotalScanned.Size = new System.Drawing.Size(10, 13);
            this.labelTotalScanned.TabIndex = 14;
            this.labelTotalScanned.Text = "-";
            // 
            // labelActualRarityInfo
            // 
            this.labelActualRarityInfo.AutoSize = true;
            this.labelActualRarityInfo.Location = new System.Drawing.Point(34, 201);
            this.labelActualRarityInfo.Name = "labelActualRarityInfo";
            this.labelActualRarityInfo.Size = new System.Drawing.Size(37, 13);
            this.labelActualRarityInfo.TabIndex = 16;
            this.labelActualRarityInfo.Text = "Rarity:";
            // 
            // labelActualRarity
            // 
            this.labelActualRarity.AutoSize = true;
            this.labelActualRarity.Location = new System.Drawing.Point(77, 201);
            this.labelActualRarity.Name = "labelActualRarity";
            this.labelActualRarity.Size = new System.Drawing.Size(10, 13);
            this.labelActualRarity.TabIndex = 15;
            this.labelActualRarity.Text = "-";
            // 
            // ScanForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 261);
            this.Controls.Add(this.labelActualRarityInfo);
            this.Controls.Add(this.labelActualRarity);
            this.Controls.Add(this.labelTotalScanned);
            this.Controls.Add(this.labelTotalScannedInfo);
            this.Controls.Add(this.labelActualNameInfo);
            this.Controls.Add(this.labelActualTypeInfo);
            this.Controls.Add(this.labelActualType);
            this.Controls.Add(this.labelActualId);
            this.Controls.Add(this.labelActualIdInfo);
            this.Controls.Add(this.labelActualPageInfo);
            this.Controls.Add(this.labelActualPage);
            this.Controls.Add(this.labelActualName);
            this.Controls.Add(this.textInitialPage);
            this.Controls.Add(this.labelInitialPageInfo);
            this.Controls.Add(this.buttonStartScanning);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximumSize = new System.Drawing.Size(300, 300);
            this.MinimumSize = new System.Drawing.Size(300, 300);
            this.Name = "ScanForm";
            this.Text = "Mtg Database";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ScanForm_FormClosing);
            this.Load += new System.EventHandler(this.ScanForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonStartScanning;
        private System.Windows.Forms.Label labelInitialPageInfo;
        private System.Windows.Forms.TextBox textInitialPage;
        private System.Windows.Forms.Label labelActualName;
        private System.Windows.Forms.Label labelActualPage;
        private System.Windows.Forms.Label labelActualPageInfo;
        private System.Windows.Forms.Label labelActualIdInfo;
        private System.Windows.Forms.Label labelActualId;
        private System.Windows.Forms.Label labelActualType;
        private System.Windows.Forms.Label labelActualTypeInfo;
        private System.Windows.Forms.Label labelActualNameInfo;
        private System.Windows.Forms.Label labelTotalScannedInfo;
        private System.Windows.Forms.Label labelTotalScanned;
        private System.Windows.Forms.Label labelActualRarityInfo;
        private System.Windows.Forms.Label labelActualRarity;
    }
}