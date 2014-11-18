namespace MARS.ControlPanel.Forms
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

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
            System.Windows.Forms.Label exportEnabledCaption;
            System.Windows.Forms.Label pluginInstalledCaption;
            this.teamSpeakPathTextBox = new System.Windows.Forms.TextBox();
            this.selectPathButton = new System.Windows.Forms.Button();
            this.saveButton = new System.Windows.Forms.Button();
            this.pluginInstalledLabel = new System.Windows.Forms.Label();
            this.exportEnabledLabel = new System.Windows.Forms.Label();
            exportEnabledCaption = new System.Windows.Forms.Label();
            pluginInstalledCaption = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // teamSpeakPathTextBox
            // 
            this.teamSpeakPathTextBox.Location = new System.Drawing.Point(13, 13);
            this.teamSpeakPathTextBox.Name = "teamSpeakPathTextBox";
            this.teamSpeakPathTextBox.ReadOnly = true;
            this.teamSpeakPathTextBox.Size = new System.Drawing.Size(256, 20);
            this.teamSpeakPathTextBox.TabIndex = 0;
            this.teamSpeakPathTextBox.TextChanged += new System.EventHandler(this.teamSpeakPathTextBox_TextChanged);
            // 
            // selectPathButton
            // 
            this.selectPathButton.Location = new System.Drawing.Point(275, 11);
            this.selectPathButton.Name = "selectPathButton";
            this.selectPathButton.Size = new System.Drawing.Size(75, 23);
            this.selectPathButton.TabIndex = 1;
            this.selectPathButton.Text = "Select";
            this.selectPathButton.UseVisualStyleBackColor = true;
            this.selectPathButton.Click += new System.EventHandler(this.selectPathButton_Click);
            // 
            // saveButton
            // 
            this.saveButton.Location = new System.Drawing.Point(275, 40);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(75, 23);
            this.saveButton.TabIndex = 2;
            this.saveButton.Text = "Save";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // exportEnabledCaption
            // 
            exportEnabledCaption.AutoSize = true;
            exportEnabledCaption.Location = new System.Drawing.Point(12, 50);
            exportEnabledCaption.Name = "exportEnabledCaption";
            exportEnabledCaption.Size = new System.Drawing.Size(81, 13);
            exportEnabledCaption.TabIndex = 4;
            exportEnabledCaption.Text = "Export enabled:";
            // 
            // pluginInstalledCaption
            // 
            pluginInstalledCaption.AutoSize = true;
            pluginInstalledCaption.Location = new System.Drawing.Point(12, 36);
            pluginInstalledCaption.Name = "pluginInstalledCaption";
            pluginInstalledCaption.Size = new System.Drawing.Size(80, 13);
            pluginInstalledCaption.TabIndex = 5;
            pluginInstalledCaption.Text = "Plugin installed:";
            // 
            // pluginInstalledLabel
            // 
            this.pluginInstalledLabel.AutoSize = true;
            this.pluginInstalledLabel.Location = new System.Drawing.Point(98, 36);
            this.pluginInstalledLabel.Name = "pluginInstalledLabel";
            this.pluginInstalledLabel.Size = new System.Drawing.Size(14, 13);
            this.pluginInstalledLabel.TabIndex = 6;
            this.pluginInstalledLabel.Text = "X";
            // 
            // exportEnabledLabel
            // 
            this.exportEnabledLabel.AutoSize = true;
            this.exportEnabledLabel.Location = new System.Drawing.Point(99, 50);
            this.exportEnabledLabel.Name = "exportEnabledLabel";
            this.exportEnabledLabel.Size = new System.Drawing.Size(14, 13);
            this.exportEnabledLabel.TabIndex = 7;
            this.exportEnabledLabel.Text = "X";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(545, 312);
            this.Controls.Add(this.exportEnabledLabel);
            this.Controls.Add(this.pluginInstalledLabel);
            this.Controls.Add(pluginInstalledCaption);
            this.Controls.Add(exportEnabledCaption);
            this.Controls.Add(this.saveButton);
            this.Controls.Add(this.selectPathButton);
            this.Controls.Add(this.teamSpeakPathTextBox);
            this.Name = "MainForm";
            this.Text = "MainForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox teamSpeakPathTextBox;
        private System.Windows.Forms.Button selectPathButton;
        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.Label pluginInstalledLabel;
        private System.Windows.Forms.Label exportEnabledLabel;
    }
}