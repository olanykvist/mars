
namespace MARS.ControlPanel.Forms
{
    using System;
    using System.IO;
    using System.Windows.Forms;
    using MARS.Common;
    using Microsoft.Win32;

    public partial class MainForm : Form
    {
        private Configuration configuration;

        public MainForm()
        {
            InitializeComponent();
            this.LoadConfiguration();
        }

        private void LoadConfiguration()
        {
            this.configuration = Configuration.Load();

            // No TS path in configuration. Guess valid path
            if (string.IsNullOrEmpty(this.configuration.TeamSpeakPath))
            {
                // Try current user first installation first
                var key = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\TeamSpeak 3 Client");
                if (key == null)
                {
                    key = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\TeamSpeak 3 Client");
                }

                if (key != null)
                {
                    this.configuration.TeamSpeakPath = key.GetValue(null) as string;
                }
            }

            this.teamSpeakPathTextBox.Text = configuration.TeamSpeakPath;
        }

        private void SaveConfiguration()
        {
            this.configuration.TeamSpeakPath = this.teamSpeakPathTextBox.Text.Trim();
            this.configuration.Save();
        }

        private bool IsPluginInstalled()
        {
            var originalFile = new FileInfo(Path.Combine(this.GetInstallPath(), "MARS.lua"));
            var installedFile = new FileInfo(Path.Combine(this.configuration.TeamSpeakPath, "plugins", "MARS.dll"));

            if (!installedFile.Exists)
            {
                return false;
            }

            if (!Utility.FilesAreEqual(originalFile, installedFile))
            {
                return false;
            }

            return true;
        }

        private bool IsScriptInstalled()
        {
            var installedFile = new FileInfo(Path.Combine(this.GetScriptsPath(), "MARS.lua"));
            var originalFile = new FileInfo(Path.Combine(this.GetInstallPath(), "MARS.lua"));

            if (!installedFile.Exists)
            {
                return false;
            }

            if (!Utility.FilesAreEqual(originalFile, installedFile))
            {
                return false;
            }

            return true;
        }

        private bool IsExportEnabled()
        {
            var export = new FileInfo(Path.Combine(this.GetScriptsPath(), "Export.lua"));

            if (!export.Exists)
            {
                return false;
            }

            using (var stream = export.OpenText())
            {
                var found = false;
                var line = string.Empty;

                while ((line = stream.ReadLine()) != null)
                {
                    if (line.Equals("dofile(lfs.writedir() .. \"Scripts/MARS.lua\")", StringComparison.InvariantCultureIgnoreCase))
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    return true;
                }

                return false;
            }
        }

        private void EnableExport()
        {
            var export = new FileInfo(Path.Combine(this.GetScriptsPath(), "Export.lua"));

        }

        private void InstallScript()
        {
            var file = "MARS.lua";
            var scriptsPath = EnsureScriptsPath();
            var installPath = GetInstallPath();
            var source = Path.Combine(installPath, file);
            var target = Path.Combine(scriptsPath, file);
            File.Copy(source, target, true);
        }

        private string EnsureScriptsPath()
        {
            var path = Path.Combine(Utility.GetKnownFolderPath(KnownFolderId.SavedGames), @"DCS\Scripts");

            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }

            return path;
        }

        private string GetScriptsPath()
        {
            return Path.Combine(Utility.GetKnownFolderPath(KnownFolderId.SavedGames), @"DCS\Scripts");
        }

        private string GetInstallPath()
        {
            return @"C:\Program Files\Master Arms\MARS";
        }

        private void selectPathButton_Click(object sender, EventArgs e)
        {
            using (var dialog = new FolderBrowserDialog())
            {
                if (Directory.Exists(this.configuration.TeamSpeakPath))
                {
                    dialog.SelectedPath = this.configuration.TeamSpeakPath;
                }

                dialog.ShowNewFolderButton = false;
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    this.teamSpeakPathTextBox.Text = dialog.SelectedPath;
                }
            }
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            this.SaveConfiguration();
        }

        private void CheckInstallation()
        {
            this.pluginInstalledLabel.Text = this.IsPluginInstalled().ToString();
            this.exportEnabledLabel.Text = this.IsExportEnabled().ToString();
        }

        private void teamSpeakPathTextBox_TextChanged(object sender, EventArgs e)
        {
            this.CheckInstallation();
        }
    }
}
