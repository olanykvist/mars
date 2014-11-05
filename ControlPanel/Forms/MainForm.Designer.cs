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
            this.teamSpeakPathTextBox = new System.Windows.Forms.TextBox();
            this.selectPathButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // teamSpeakPathTextBox
            // 
            this.teamSpeakPathTextBox.Location = new System.Drawing.Point(13, 13);
            this.teamSpeakPathTextBox.Name = "teamSpeakPathTextBox";
            this.teamSpeakPathTextBox.Size = new System.Drawing.Size(100, 20);
            this.teamSpeakPathTextBox.TabIndex = 0;
            // 
            // selectPathButton
            // 
            this.selectPathButton.Location = new System.Drawing.Point(119, 11);
            this.selectPathButton.Name = "selectPathButton";
            this.selectPathButton.Size = new System.Drawing.Size(75, 23);
            this.selectPathButton.TabIndex = 1;
            this.selectPathButton.Text = "Select";
            this.selectPathButton.UseVisualStyleBackColor = true;
            this.selectPathButton.Click += new System.EventHandler(this.selectPathButton_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(545, 312);
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
    }
}