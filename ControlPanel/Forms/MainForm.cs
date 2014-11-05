
namespace MARS.ControlPanel.Forms
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Data;
    using System.Drawing;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    using System.Windows.Forms;
    using MARS.Common;

    public partial class MainForm : Form
    {
        private Configuration configuration;

        public MainForm()
        {
            InitializeComponent();
            configuration = Configuration.Load();
            this.teamSpeakPathTextBox.Text = configuration.TeamSpeakPath;
        }

        private void selectPathButton_Click(object sender, EventArgs e)
        {
            using (var dialog = new FolderBrowserDialog())
            {
                dialog.ShowNewFolderButton = false;
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    this.teamSpeakPathTextBox.Text = dialog.SelectedPath;
                }
            }
        }
    }
}
