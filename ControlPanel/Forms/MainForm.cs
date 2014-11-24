
namespace MARS.ControlPanel.Forms
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.IO;
    using System.Threading;
    using System.Windows.Forms;
    using MARS.Common;
    using Microsoft.Win32;
    using SharpDX.DirectInput;
    using SharpDX.Multimedia;

    public partial class MainForm : Form
    {
        private Configuration configuration;
        private DirectInput input;
        private IList<Device> devices;

        public MainForm()
        {
            this.InitializeComponent();
            this.LoadConfiguration();
            this.InitializeInput();
        }

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (this.components != null))
            {
                this.components.Dispose();
            }

            foreach (var device in this.devices)
            {
                if (device != null)
                {
                    device.Unacquire();
                    device.Dispose();
                }
            }

            if (this.input != null)
            {
                this.input.Dispose();
            }

            base.Dispose(disposing);
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

            this.teamSpeakPathTextBox.Text = this.configuration.TeamSpeakPath;
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
            var scriptsPath = this.EnsureScriptsPath();
            var installPath = this.GetInstallPath();
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

        private void CheckInstallation()
        {
            this.pluginInstalledLabel.Text = this.IsPluginInstalled().ToString();
            this.exportEnabledLabel.Text = this.IsExportEnabled().ToString();
        }

        private void InitializeInput()
        {
            this.input = new DirectInput();
            this.devices = new List<Device>();

            var instances = this.input.GetDevices(DeviceClass.GameControl, DeviceEnumerationFlags.AttachedOnly);

            foreach (var instance in instances)
            {
                if (instance.Usage == UsageId.GenericJoystick)
                {
                    var device = new Joystick(this.input, instance.ProductGuid);
                    device.SetCooperativeLevel(this, CooperativeLevel.Background | CooperativeLevel.NonExclusive);
                    device.Acquire();

                    this.devices.Add(device);
                }
            }
        }

        private void ToggleSetSelectPttButtons()
        {
            this.setSelectPttOneButton.Enabled = !this.setSelectPttOneButton.Enabled;
            this.setSelectPttTwoButton.Enabled = !this.setSelectPttTwoButton.Enabled;
            this.setSelectPttThreeButton.Enabled = !this.setSelectPttThreeButton.Enabled;
        }

        private void OnSelectPathButtonClick(object sender, EventArgs e)
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

        private void OnSaveButtonClick(object sender, EventArgs e)
        {
            this.SaveConfiguration();
        }

        private void OnSetSelectPttButtonClick(object sender, EventArgs e)
        {
            this.ToggleSetSelectPttButtons();
            var assigment = (sender as Control).Tag.ToString();
            this.inputListener.RunWorkerAsync(assigment);
        }

        private void OnTeamSpeakPathTextBoxTextChanged(object sender, EventArgs e)
        {
            this.CheckInstallation();
        }

        private void OnInputListenerDoWork(object sender, DoWorkEventArgs e)
        {
            bool[,] initial = new bool[this.devices.Count, 128];

            for (int i = 0; i < this.devices.Count; i++)
            {
                this.devices[i].Poll();
                var state = (this.devices[i] as Joystick).GetCurrentState();

                for (int j = 0; j < state.Buttons.Length; j++)
                {
                    initial[i, j] = state.Buttons[j];
                }
            }

            string device = string.Empty;
            int button = 0;
            bool found = false;

            while (!found)
            {
                Thread.Sleep(100);

                for (int i = 0; i < this.devices.Count; i++)
                {
                    this.devices[i].Poll();
                    var state = (this.devices[i] as Joystick).GetCurrentState();

                    for (int j = 0; j < state.Buttons.Length; j++)
                    {
                        if (state.Buttons[j] && !initial[i, j])
                        {
                            found = true;
                            button = j;
                            device = this.devices[i].Information.ProductName;
                            break;
                        }
                    }

                    if (found)
                    {
                        break;
                    }
                }
            }

            e.Result = new InputAssignment { AssignmentName = e.Argument.ToString(), Device = device, Button = button };
        }

        private void OnInputListenerRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            var assignment = e.Result as InputAssignment;

            if (assignment.AssignmentName.Equals("SELECT_PTT_1", StringComparison.InvariantCulture))
            {
                this.selectPttOneDeviceLabel.Text = assignment.Device;
                this.selectPttOneButtonLabel.Text = assignment.Button.ToString();
            }
            else if (assignment.AssignmentName.Equals("SELECT_PTT_2", StringComparison.InvariantCulture))
            {
                this.selectPttTwoDeviceLabel.Text = assignment.Device;
                this.selectPttTwoButtonLabel.Text = assignment.Button.ToString();
            }
            else if (assignment.AssignmentName.Equals("SELECT_PTT_3", StringComparison.InvariantCulture))
            {
                this.selectPttThreeDeviceLabel.Text = assignment.Device;
                this.selectPttThreeButtonLabel.Text = assignment.Button.ToString();
            }

            this.ToggleSetSelectPttButtons();
        }
    }
}
