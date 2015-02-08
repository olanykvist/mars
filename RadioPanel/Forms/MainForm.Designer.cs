namespace MARS.RadioPanel.Forms
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
            this.components = new System.ComponentModel.Container();
            this.connectionTimer = new System.Windows.Forms.Timer(this.components);
            this.mainStatusStrip = new System.Windows.Forms.StatusStrip();
            this.mainSatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.radio3 = new MARS.RadioPanel.Controls.Radio();
            this.radio2 = new MARS.RadioPanel.Controls.Radio();
            this.radio1 = new MARS.RadioPanel.Controls.Radio();
            this.mainStatusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // connectionTimer
            // 
            this.connectionTimer.Interval = 3000;
            this.connectionTimer.Tick += new System.EventHandler(this.connectionTimer_Tick);
            // 
            // mainStatusStrip
            // 
            this.mainStatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mainSatusLabel});
            this.mainStatusStrip.Location = new System.Drawing.Point(0, 319);
            this.mainStatusStrip.Name = "mainStatusStrip";
            this.mainStatusStrip.Size = new System.Drawing.Size(182, 22);
            this.mainStatusStrip.SizingGrip = false;
            this.mainStatusStrip.TabIndex = 0;
            this.mainStatusStrip.Text = "statusStrip1";
            // 
            // mainSatusLabel
            // 
            this.mainSatusLabel.Name = "mainSatusLabel";
            this.mainSatusLabel.Size = new System.Drawing.Size(42, 17);
            this.mainSatusLabel.Text = "Status:";
            // 
            // radio3
            // 
            this.radio3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.radio3.Frequency = 40500000;
            this.radio3.Id = 3;
            this.radio3.Location = new System.Drawing.Point(12, 214);
            this.radio3.Modulation = MARS.RadioPanel.Controls.Modulation.FM;
            this.radio3.Name = "radio3";
            this.radio3.Size = new System.Drawing.Size(163, 95);
            this.radio3.TabIndex = 3;
            this.radio3.FrequencyChanged += new System.EventHandler(this.OnFrequencyChanged);
            // 
            // radio2
            // 
            this.radio2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.radio2.Frequency = 243000000;
            this.radio2.Id = 2;
            this.radio2.Location = new System.Drawing.Point(12, 113);
            this.radio2.Modulation = MARS.RadioPanel.Controls.Modulation.AM;
            this.radio2.Name = "radio2";
            this.radio2.Size = new System.Drawing.Size(163, 95);
            this.radio2.TabIndex = 2;
            this.radio2.FrequencyChanged += new System.EventHandler(this.OnFrequencyChanged);
            // 
            // radio1
            // 
            this.radio1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.radio1.Frequency = 121500000;
            this.radio1.Id = 1;
            this.radio1.Location = new System.Drawing.Point(12, 12);
            this.radio1.Modulation = MARS.RadioPanel.Controls.Modulation.AM;
            this.radio1.Name = "radio1";
            this.radio1.Size = new System.Drawing.Size(163, 95);
            this.radio1.TabIndex = 1;
            this.radio1.FrequencyChanged += new System.EventHandler(this.OnFrequencyChanged);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(182, 341);
            this.Controls.Add(this.radio3);
            this.Controls.Add(this.radio2);
            this.Controls.Add(this.radio1);
            this.Controls.Add(this.mainStatusStrip);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainForm";
            this.Text = "MARS Radio Panel";
            this.mainStatusStrip.ResumeLayout(false);
            this.mainStatusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Timer connectionTimer;
        private System.Windows.Forms.StatusStrip mainStatusStrip;
        private System.Windows.Forms.ToolStripStatusLabel mainSatusLabel;
        private Controls.Radio radio1;
        private Controls.Radio radio2;
        private Controls.Radio radio3;
    }
}