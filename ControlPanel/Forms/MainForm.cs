
namespace MARS.ControlPanel.Forms
{
    using System;
    using System.IO;
    using System.Windows.Forms;
    using MARS.Common;
    using Microsoft.Win32;
    using SharpDX.DirectInput;
    using System.ComponentModel;
    using System.Threading;
    using System.Collections.Generic;
    using System.Diagnostics;



    public partial class MainForm : Form
    {
        private Configuration configuration;
        private DirectInput input;
        private IList<Device> devices;

        public MainForm()
        {
            InitializeComponent();
            this.LoadConfiguration();
            this.InitInput();
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

        private void setPTT1Button_Click(object sender, EventArgs e)
        {
            this.inputListener.RunWorkerAsync(1);
        }

        private void inputListener_DoWork(object sender, DoWorkEventArgs e)
        {
            var initial = new bool[devices.Count, 128];

            for (int i = 0; i < devices.Count; i++)
            {
                devices[i].Acquire();
                devices[i].Poll();
                var state = (devices[i] as Joystick).GetCurrentState();
                for (int j = 0; j < 128; j++)
                {
                    initial[i, j] = state.Buttons[j];
                }
            }

            string device = string.Empty;
            int button = 0;
            bool exit = false;
            do
            {
                for (int i = 0; i < devices.Count; i++)
                {
                    devices[i].Poll();
                    var state = (devices[i] as Joystick).GetCurrentState();
                    for (int j = 0; j < 128; j++)
                    {
                        if (state.Buttons[j] == true && initial[i, j] == false)
                        {
                            exit = true;
                            device = devices[i].Information.ProductName;
                            button = j;
                            break;
                        }
                    }
                    if (exit)
                    {
                        break;
                    }
                }
                Thread.Sleep(10);
            } while (!exit);

            e.Result = new InputAssignment { Device = device, Button = button };
        }

        private void inputListener_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            InputAssignment assignment = e.Result as InputAssignment;
            this.button1DeviceLabel.Text = assignment.Device;
            this.button1ButtonLabel.Text = assignment.Button.ToString();
        }

        private void InitInput()
        {
            this.input = new DirectInput();
            this.devices = new List<Device>();

            var instances = input.GetDevices(DeviceClass.GameControl, DeviceEnumerationFlags.AttachedOnly);

            foreach (var instance in instances)
            {
                this.devices.Add(new Joystick(input, instance.ProductGuid));
            }

            foreach (var device in devices)
            {
                device.SetCooperativeLevel(this, CooperativeLevel.Background | CooperativeLevel.NonExclusive);
            }
        }

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
    }

    class InputAssignment
    {
        public string Device { get; set; }

        public int Button { get; set; }
    }
}
