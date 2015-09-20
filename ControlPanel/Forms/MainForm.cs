
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
        const string EXPORT = "dofile(lfs.writedir() .. \"Scripts/MARS.lua\")";
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

            this.selectPttOneDeviceLabel.Text = this.configuration.SelectPttOneDevice;
            this.selectPttOneButtonLabel.Text = this.configuration.SelectPttOneButton.ToString();

            this.selectPttTwoDeviceLabel.Text = this.configuration.SelectPttTwoDevice;
            this.selectPttTwoButtonLabel.Text = this.configuration.SelectPttTwoButton.ToString();

            this.selectPttThreeDeviceLabel.Text = this.configuration.SelectPttThreeDevice;
            this.selectPttThreeButtonLabel.Text = this.configuration.SelectPttThreeButton.ToString();

            this.pttCommonDeviceLabel.Text = this.configuration.PttCommonDevice;
            this.pttCommonButtonLabel.Text = this.configuration.PttCommonButton.ToString();

            this.radioOnePanTrack.Value = (int)(this.configuration.RadioOnePan * 100);
            this.radioTwoPanTrack.Value = (int)(this.configuration.RadioTwoPan * 100);
            this.radioThreePanTrack.Value = (int)(this.configuration.RadioThreePan * 100);
        }

        private void SaveConfiguration()
        {
            this.configuration.TeamSpeakPath = this.teamSpeakPathTextBox.Text.Trim();

            this.configuration.SelectPttOneDevice = this.selectPttOneDeviceLabel.Text;
            this.configuration.SelectPttOneButton = Convert.ToInt32(this.selectPttOneButtonLabel.Text);

            this.configuration.SelectPttTwoDevice = this.selectPttTwoDeviceLabel.Text;
            this.configuration.SelectPttTwoButton = Convert.ToInt32(this.selectPttTwoButtonLabel.Text);

            this.configuration.SelectPttThreeDevice = this.selectPttThreeDeviceLabel.Text;
            this.configuration.SelectPttThreeButton = Convert.ToInt32(this.selectPttThreeButtonLabel.Text);

            this.configuration.PttCommonDevice = this.pttCommonDeviceLabel.Text;
            this.configuration.PttCommonButton = Convert.ToInt32(this.pttCommonButtonLabel.Text);

            this.configuration.RadioOnePan = this.radioOnePanTrack.Value / 100.0f;
            this.configuration.RadioTwoPan = this.radioTwoPanTrack.Value / 100.0f;
            this.configuration.RadioThreePan = this.radioThreePanTrack.Value / 100.0f;

            this.configuration.Save();
        }

        private bool IsPluginInstalled()
        {
            var originalFile = new FileInfo(Path.Combine(this.GetInstallPath(), @"Plugin\MARS.dll"));
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
            var originalFile = new FileInfo(Path.Combine(this.GetInstallPath(), @"Scripts\MARS.lua"));
            var installedFile = new FileInfo(Path.Combine(this.EnsureScriptsPath(), "MARS.lua"));

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
            var export = new FileInfo(Path.Combine(this.EnsureScriptsPath(), "Export.lua"));

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
                    if (line.Equals(EXPORT, StringComparison.InvariantCultureIgnoreCase))
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
            var export = new FileInfo(Path.Combine(this.EnsureScriptsPath(), "Export.lua"));

            if (!export.Exists)
            {
                var source = Path.Combine(this.GetInstallPath(), "Scripts", "Export.lua");
                var target = Path.Combine(this.EnsureScriptsPath(), "Export.lua");
                File.Copy(source, target, true);

                return;
            }

            var lines = File.ReadAllLines(Path.Combine(this.EnsureScriptsPath(), "Export.lua"));
            var found = false;
            for (int i = 0; i < lines.Length; ++i)
            {
                if (lines[i].Contains(EXPORT))
                {
                    found = true;
                    lines[i] = EXPORT;
                }
            }

            if (found)
            {
                File.WriteAllLines(Path.Combine(this.EnsureScriptsPath(), "Export.lua"), lines);
            }
            else
            {
                using (var stream = export.Open(FileMode.Append, FileAccess.Write, FileShare.None))
                {
                    using (var writer = new StreamWriter(stream))
                    {
                        writer.WriteLine();
                        writer.WriteLine(EXPORT);
                    }
                }
            }
        }

        private void DisableExport()
        {
            var lines = File.ReadAllLines(Path.Combine(this.EnsureScriptsPath(), "Export.lua"));

            for (int i = 0; i < lines.Length; ++i)
            {
                if (lines[i].Equals(EXPORT, StringComparison.InvariantCulture))
                {
                    lines[i] = "--" + EXPORT;
                }
            }

            File.WriteAllLines(Path.Combine(this.EnsureScriptsPath(), "Export.lua"), lines);
        }

        private void InstallScript()
        {
            var source = Path.Combine(this.GetInstallPath(), "Scripts", "MARS.lua");
            var target = Path.Combine(this.EnsureScriptsPath(), "MARS.lua");

            try
            {
                File.Copy(source, target, true);
            }
            catch (UnauthorizedAccessException)
            {
                MessageBox.Show("Permission denied :-(", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void InstallPlugin()
        {
            var source = Path.Combine(this.GetInstallPath(), "Plugin", "MARS.dll");
            var target = Path.Combine(this.configuration.TeamSpeakPath, "plugins", "MARS.dll");

            try
            {
                File.Copy(source, target, true);
            }
            catch (UnauthorizedAccessException)
            {
                MessageBox.Show("Permission denied :-(\n Change permissions on the TeamSpeak plugins folder, or run this program as administrator", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            catch (IOException exception)
            {
                MessageBox.Show("Critical error during file copy: " + exception.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
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

        private string GetInstallPath()
        {
            var key = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Master Arms\MARS");
            var path = key.GetValue("InstallPath") as string;
            return path;
        }

        private void CheckInstallation()
        {
            this.pluginInstalledLabel.Text = this.IsPluginInstalled() ? "Yes" : "No";
            this.scriptInstalledLabel.Text = this.IsScriptInstalled() ? "Yes" : "No";
            this.exportEnabledLabel.Text = this.IsExportEnabled() ? "Yes" : "No";

            this.installPluginButton.Enabled = !this.IsPluginInstalled();
            this.installScriptButton.Enabled = !this.IsScriptInstalled();
            this.enableExportButton.Enabled = !this.IsExportEnabled();
            this.disableExportButton.Enabled = this.IsExportEnabled();
        }

        private void InitializeInput()
        {
            this.input = new DirectInput();
            this.devices = new List<Device>();
            var instances = this.input.GetDevices();

            foreach (var instance in instances)
            {
                var device = null as Device;

                if (instance.Usage == UsageId.GenericJoystick)
                {
                    device = new Joystick(this.input, instance.ProductGuid);
                }
                else if (instance.Type == DeviceType.Keyboard)
                {
                    device = new Keyboard(this.input);
                }

                if (device != null)
                {
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
            this.setPttCommonButton.Enabled = !this.setPttCommonButton.Enabled;
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
            this.configuration.TeamSpeakPath = this.teamSpeakPathTextBox.Text;
            this.CheckInstallation();
        }

        private void OnInputListenerDoWork(object sender, DoWorkEventArgs e)
        {
            bool[,] initial = new bool[this.devices.Count, 128];

            for (int i = 0; i < this.devices.Count; i++)
            {
                this.devices[i].Poll();

                if (this.devices[i] is Joystick)
                {
                    var state = (this.devices[i] as Joystick).GetCurrentState();

                    for (int j = 0; j < 128; j++)
                    {
                        initial[i, j] = state.Buttons[j];
                    }
                }
                else if (this.devices[i] is Keyboard)
                {
                    var state = (this.devices[i] as Keyboard).GetCurrentState();

                    for (int j = 0; j < 128; j++)
                    {
                        initial[i, j] = state.IsPressed(state.AllKeys[j]);
                    }
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

                    if (this.devices[i] is Joystick)
                    {
                        var state = (this.devices[i] as Joystick).GetCurrentState();

                        for (int j = 0; j < 128; j++)
                        {
                            if (state.Buttons[j] && !initial[i, j])
                            {
                                found = true;
                                button = j;
                                device = this.devices[i].Information.ProductName;
                                break;
                            }
                        }
                    }
                    else if (this.devices[i] is Keyboard)
                    {
                        var state = (this.devices[i] as Keyboard).GetCurrentState();

                        for (int j = 0; j < 128; j++)
                        {
                            if (state.IsPressed(state.AllKeys[j]) && !initial[i, j])
                            {
                                found = true;
                                button = j;
                                device = this.devices[i].Information.ProductName;
                                break;
                            }
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
            else if (assignment.AssignmentName.Equals("COMMON_PTT", StringComparison.InvariantCulture))
            {
                this.pttCommonDeviceLabel.Text = assignment.Device;
                this.pttCommonButtonLabel.Text = assignment.Button.ToString();
            }

            this.ToggleSetSelectPttButtons();
        }

        private void OnMainFormClosing(object sender, FormClosingEventArgs e)
        {
            this.SaveConfiguration();
        }

        private void OnInstallPluginButtonClick(object sender, EventArgs e)
        {
            this.InstallPlugin();
            this.CheckInstallation();
        }

        private void OnInstallScriptButtonClick(object sender, EventArgs e)
        {
            this.InstallScript();
            this.CheckInstallation();
        }

        private void OnEnableExportButtonClick(object sender, EventArgs e)
        {
            this.EnableExport();
            this.CheckInstallation();
        }

        private void OnDisableExportButtonClick(object sender, EventArgs e)
        {
            this.DisableExport();
            this.CheckInstallation();
        }
    }
}
