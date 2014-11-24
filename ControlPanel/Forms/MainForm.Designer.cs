namespace MARS.ControlPanel.Forms
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.Label exportEnabledCaption;
            System.Windows.Forms.Label pluginInstalledCaption;
            System.Windows.Forms.Label selectPttOneDeviceCaption;
            System.Windows.Forms.Label selectPttOneButtonCaption;
            System.Windows.Forms.Label selectPttTwoButtonCaption;
            System.Windows.Forms.Label selectPttTwoDeviceCaption;
            System.Windows.Forms.Label selectPttThreeButtonCaption;
            System.Windows.Forms.Label selectPttThreeDeviceCaption;
            this.teamSpeakPathTextBox = new System.Windows.Forms.TextBox();
            this.selectPathButton = new System.Windows.Forms.Button();
            this.saveButton = new System.Windows.Forms.Button();
            this.pluginInstalledLabel = new System.Windows.Forms.Label();
            this.exportEnabledLabel = new System.Windows.Forms.Label();
            this.setSelectPttOneButton = new System.Windows.Forms.Button();
            this.inputListener = new System.ComponentModel.BackgroundWorker();
            this.selectPttOneDeviceLabel = new System.Windows.Forms.Label();
            this.selectPttOneButtonLabel = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.selectPttThreeButtonLabel = new System.Windows.Forms.Label();
            this.selectPttThreeDeviceLabel = new System.Windows.Forms.Label();
            this.setSelectPttThreeButton = new System.Windows.Forms.Button();
            this.selectPttTwoButtonLabel = new System.Windows.Forms.Label();
            this.selectPttTwoDeviceLabel = new System.Windows.Forms.Label();
            this.setSelectPttTwoButton = new System.Windows.Forms.Button();
            this.devicesListBox = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            exportEnabledCaption = new System.Windows.Forms.Label();
            pluginInstalledCaption = new System.Windows.Forms.Label();
            selectPttOneDeviceCaption = new System.Windows.Forms.Label();
            selectPttOneButtonCaption = new System.Windows.Forms.Label();
            selectPttTwoButtonCaption = new System.Windows.Forms.Label();
            selectPttTwoDeviceCaption = new System.Windows.Forms.Label();
            selectPttThreeButtonCaption = new System.Windows.Forms.Label();
            selectPttThreeDeviceCaption = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
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
            // selectPttOneDeviceCaption
            // 
            selectPttOneDeviceCaption.AutoSize = true;
            selectPttOneDeviceCaption.Location = new System.Drawing.Point(30, 43);
            selectPttOneDeviceCaption.Name = "selectPttOneDeviceCaption";
            selectPttOneDeviceCaption.Size = new System.Drawing.Size(44, 13);
            selectPttOneDeviceCaption.TabIndex = 9;
            selectPttOneDeviceCaption.Text = "Device:";
            // 
            // selectPttOneButtonCaption
            // 
            selectPttOneButtonCaption.AutoSize = true;
            selectPttOneButtonCaption.Location = new System.Drawing.Point(30, 60);
            selectPttOneButtonCaption.Name = "selectPttOneButtonCaption";
            selectPttOneButtonCaption.Size = new System.Drawing.Size(41, 13);
            selectPttOneButtonCaption.TabIndex = 10;
            selectPttOneButtonCaption.Text = "Button:";
            // 
            // selectPttTwoButtonCaption
            // 
            selectPttTwoButtonCaption.AutoSize = true;
            selectPttTwoButtonCaption.Location = new System.Drawing.Point(30, 105);
            selectPttTwoButtonCaption.Name = "selectPttTwoButtonCaption";
            selectPttTwoButtonCaption.Size = new System.Drawing.Size(41, 13);
            selectPttTwoButtonCaption.TabIndex = 15;
            selectPttTwoButtonCaption.Text = "Button:";
            // 
            // selectPttTwoDeviceCaption
            // 
            selectPttTwoDeviceCaption.AutoSize = true;
            selectPttTwoDeviceCaption.Location = new System.Drawing.Point(30, 88);
            selectPttTwoDeviceCaption.Name = "selectPttTwoDeviceCaption";
            selectPttTwoDeviceCaption.Size = new System.Drawing.Size(44, 13);
            selectPttTwoDeviceCaption.TabIndex = 13;
            selectPttTwoDeviceCaption.Text = "Device:";
            // 
            // selectPttThreeButtonCaption
            // 
            selectPttThreeButtonCaption.AutoSize = true;
            selectPttThreeButtonCaption.Location = new System.Drawing.Point(30, 151);
            selectPttThreeButtonCaption.Name = "selectPttThreeButtonCaption";
            selectPttThreeButtonCaption.Size = new System.Drawing.Size(41, 13);
            selectPttThreeButtonCaption.TabIndex = 20;
            selectPttThreeButtonCaption.Text = "Button:";
            // 
            // selectPttThreeDeviceCaption
            // 
            selectPttThreeDeviceCaption.AutoSize = true;
            selectPttThreeDeviceCaption.Location = new System.Drawing.Point(30, 134);
            selectPttThreeDeviceCaption.Name = "selectPttThreeDeviceCaption";
            selectPttThreeDeviceCaption.Size = new System.Drawing.Size(44, 13);
            selectPttThreeDeviceCaption.TabIndex = 18;
            selectPttThreeDeviceCaption.Text = "Device:";
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
            // setSelectPttOneButton
            // 
            this.setSelectPttOneButton.Location = new System.Drawing.Point(295, 55);
            this.setSelectPttOneButton.Name = "setSelectPttOneButton";
            this.setSelectPttOneButton.Size = new System.Drawing.Size(75, 23);
            this.setSelectPttOneButton.TabIndex = 8;
            this.setSelectPttOneButton.Tag = "SELECT_PTT_1";
            this.setSelectPttOneButton.Text = "Set";
            this.setSelectPttOneButton.UseVisualStyleBackColor = true;
            this.setSelectPttOneButton.Click += new System.EventHandler(this.SetSelectPttButton_Click);
            // 
            // inputListener
            // 
            this.inputListener.DoWork += new System.ComponentModel.DoWorkEventHandler(this.inputListener_DoWork);
            this.inputListener.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.inputListener_RunWorkerCompleted);
            // 
            // selectPttOneDeviceLabel
            // 
            this.selectPttOneDeviceLabel.AutoSize = true;
            this.selectPttOneDeviceLabel.Location = new System.Drawing.Point(81, 43);
            this.selectPttOneDeviceLabel.Name = "selectPttOneDeviceLabel";
            this.selectPttOneDeviceLabel.Size = new System.Drawing.Size(22, 13);
            this.selectPttOneDeviceLabel.TabIndex = 9;
            this.selectPttOneDeviceLabel.Text = "xxx";
            // 
            // selectPttOneButtonLabel
            // 
            this.selectPttOneButtonLabel.AutoSize = true;
            this.selectPttOneButtonLabel.Location = new System.Drawing.Point(81, 60);
            this.selectPttOneButtonLabel.Name = "selectPttOneButtonLabel";
            this.selectPttOneButtonLabel.Size = new System.Drawing.Size(22, 13);
            this.selectPttOneButtonLabel.TabIndex = 10;
            this.selectPttOneButtonLabel.Text = "xxx";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(selectPttThreeButtonCaption);
            this.groupBox1.Controls.Add(this.selectPttThreeButtonLabel);
            this.groupBox1.Controls.Add(selectPttThreeDeviceCaption);
            this.groupBox1.Controls.Add(this.selectPttThreeDeviceLabel);
            this.groupBox1.Controls.Add(this.setSelectPttThreeButton);
            this.groupBox1.Controls.Add(selectPttTwoButtonCaption);
            this.groupBox1.Controls.Add(this.selectPttTwoButtonLabel);
            this.groupBox1.Controls.Add(selectPttTwoDeviceCaption);
            this.groupBox1.Controls.Add(this.selectPttTwoDeviceLabel);
            this.groupBox1.Controls.Add(this.setSelectPttTwoButton);
            this.groupBox1.Controls.Add(this.devicesListBox);
            this.groupBox1.Controls.Add(selectPttOneButtonCaption);
            this.groupBox1.Controls.Add(this.selectPttOneButtonLabel);
            this.groupBox1.Controls.Add(selectPttOneDeviceCaption);
            this.groupBox1.Controls.Add(this.selectPttOneDeviceLabel);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.setSelectPttOneButton);
            this.groupBox1.Location = new System.Drawing.Point(56, 155);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(376, 267);
            this.groupBox1.TabIndex = 11;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Buttons";
            // 
            // selectPttThreeButtonLabel
            // 
            this.selectPttThreeButtonLabel.AutoSize = true;
            this.selectPttThreeButtonLabel.Location = new System.Drawing.Point(81, 151);
            this.selectPttThreeButtonLabel.Name = "selectPttThreeButtonLabel";
            this.selectPttThreeButtonLabel.Size = new System.Drawing.Size(22, 13);
            this.selectPttThreeButtonLabel.TabIndex = 21;
            this.selectPttThreeButtonLabel.Text = "xxx";
            // 
            // selectPttThreeDeviceLabel
            // 
            this.selectPttThreeDeviceLabel.AutoSize = true;
            this.selectPttThreeDeviceLabel.Location = new System.Drawing.Point(81, 134);
            this.selectPttThreeDeviceLabel.Name = "selectPttThreeDeviceLabel";
            this.selectPttThreeDeviceLabel.Size = new System.Drawing.Size(22, 13);
            this.selectPttThreeDeviceLabel.TabIndex = 19;
            this.selectPttThreeDeviceLabel.Text = "xxx";
            // 
            // setSelectPttThreeButton
            // 
            this.setSelectPttThreeButton.Location = new System.Drawing.Point(295, 146);
            this.setSelectPttThreeButton.Name = "setSelectPttThreeButton";
            this.setSelectPttThreeButton.Size = new System.Drawing.Size(75, 23);
            this.setSelectPttThreeButton.TabIndex = 17;
            this.setSelectPttThreeButton.Tag = "SELECT_PTT_3";
            this.setSelectPttThreeButton.Text = "Set";
            this.setSelectPttThreeButton.UseVisualStyleBackColor = true;
            this.setSelectPttThreeButton.Click += new System.EventHandler(this.SetSelectPttButton_Click);
            // 
            // selectPttTwoButtonLabel
            // 
            this.selectPttTwoButtonLabel.AutoSize = true;
            this.selectPttTwoButtonLabel.Location = new System.Drawing.Point(81, 105);
            this.selectPttTwoButtonLabel.Name = "selectPttTwoButtonLabel";
            this.selectPttTwoButtonLabel.Size = new System.Drawing.Size(22, 13);
            this.selectPttTwoButtonLabel.TabIndex = 16;
            this.selectPttTwoButtonLabel.Text = "xxx";
            // 
            // selectPttTwoDeviceLabel
            // 
            this.selectPttTwoDeviceLabel.AutoSize = true;
            this.selectPttTwoDeviceLabel.Location = new System.Drawing.Point(81, 88);
            this.selectPttTwoDeviceLabel.Name = "selectPttTwoDeviceLabel";
            this.selectPttTwoDeviceLabel.Size = new System.Drawing.Size(22, 13);
            this.selectPttTwoDeviceLabel.TabIndex = 14;
            this.selectPttTwoDeviceLabel.Text = "xxx";
            // 
            // setSelectPttTwoButton
            // 
            this.setSelectPttTwoButton.Location = new System.Drawing.Point(295, 100);
            this.setSelectPttTwoButton.Name = "setSelectPttTwoButton";
            this.setSelectPttTwoButton.Size = new System.Drawing.Size(75, 23);
            this.setSelectPttTwoButton.TabIndex = 12;
            this.setSelectPttTwoButton.Tag = "SELECT_PTT_2";
            this.setSelectPttTwoButton.Text = "Set";
            this.setSelectPttTwoButton.UseVisualStyleBackColor = true;
            this.setSelectPttTwoButton.Click += new System.EventHandler(this.SetSelectPttButton_Click);
            // 
            // devicesListBox
            // 
            this.devicesListBox.FormattingEnabled = true;
            this.devicesListBox.Location = new System.Drawing.Point(9, 192);
            this.devicesListBox.Name = "devicesListBox";
            this.devicesListBox.Size = new System.Drawing.Size(361, 69);
            this.devicesListBox.TabIndex = 11;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(79, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Select + PTT 1";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(545, 434);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.exportEnabledLabel);
            this.Controls.Add(this.pluginInstalledLabel);
            this.Controls.Add(pluginInstalledCaption);
            this.Controls.Add(exportEnabledCaption);
            this.Controls.Add(this.saveButton);
            this.Controls.Add(this.selectPathButton);
            this.Controls.Add(this.teamSpeakPathTextBox);
            this.Name = "MainForm";
            this.Text = "MainForm";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox teamSpeakPathTextBox;
        private System.Windows.Forms.Button selectPathButton;
        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.Label pluginInstalledLabel;
        private System.Windows.Forms.Label exportEnabledLabel;
        private System.Windows.Forms.Button setSelectPttOneButton;
        private System.ComponentModel.BackgroundWorker inputListener;
        private System.Windows.Forms.Label selectPttOneDeviceLabel;
        private System.Windows.Forms.Label selectPttOneButtonLabel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label selectPttThreeButtonLabel;
        private System.Windows.Forms.Label selectPttThreeDeviceLabel;
        private System.Windows.Forms.Button setSelectPttThreeButton;
        private System.Windows.Forms.Label selectPttTwoButtonLabel;
        private System.Windows.Forms.Label selectPttTwoDeviceLabel;
        private System.Windows.Forms.Button setSelectPttTwoButton;
        private System.Windows.Forms.ListBox devicesListBox;
    }
}