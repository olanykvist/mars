//-----------------------------------------------------------------------
// <copyright file="Configuration.cs" company="Master Arms">
//     Copyright (c) Master Arms. All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace MARS.Common
{
    using System;
    using System.Globalization;
    using System.IO;

    /// <summary>
    /// Class encapsulating MARS configuration values
    /// </summary>
    public class Configuration
    {
        /// <summary>
        /// Name of the folder containing the config file
        /// </summary>
        private static readonly string FolderName = "MARS";

        /// <summary>
        /// Name of the config file
        /// </summary>
        private static readonly string FileName = "MARS.ini";

        /// <summary>
        /// Gets or sets the path to the used TS installation
        /// </summary>
        public string TeamSpeakPath { get; set; }

        /// <summary>
        /// Gets or sets the name of the input device
        /// </summary>
        public string SelectPttOneDevice { get; set; }

        /// <summary>
        /// Gets or sets the button index
        /// </summary>
        public int SelectPttOneButton { get; set; }

        /// <summary>
        /// Gets or sets the name of the input device
        /// </summary>
        public string SelectPttTwoDevice { get; set; }

        /// <summary>
        /// Gets or sets the button index
        /// </summary>
        public int SelectPttTwoButton { get; set; }

        /// <summary>
        /// Gets or sets the name of the input device
        /// </summary>
        public string SelectPttThreeDevice { get; set; }

        /// <summary>
        /// Gets or sets the button index
        /// </summary>
        public int SelectPttThreeButton { get; set; }

        /// <summary>
        /// Gets or sets the name of the input device
        /// </summary>
        public string PttCommonDevice { get; set; }

        /// <summary>
        /// Gets or sets the button index
        /// </summary>
        public int PttCommonButton { get; set; }

        /// <summary>
        /// Gets or sets the radio pan setting
        /// </summary>
        public float RadioOnePan { get; set; }

        /// <summary>
        /// Gets or sets the radio pan setting
        /// </summary>
        public float RadioTwoPan { get; set; }

        /// <summary>
        /// Gets or sets the radio pan setting
        /// </summary>
        public float RadioThreePan { get; set; }

        /// <summary>
        /// Loads current configuration from file
        /// </summary>
        /// <returns>The current configuration</returns>
        public static Configuration Load()
        {
            var config = new Configuration();
            var path = Configuration.GetIniFilePath();

            config.TeamSpeakPath = Utility.ReadConfigurationString("General", "TeamSpeakPath", string.Empty, path);

            config.SelectPttOneDevice = Utility.ReadConfigurationString("SELECT_PTT_1", "Device", string.Empty, path);
            config.SelectPttOneButton = Utility.ReadConfigurationInteger("SELECT_PTT_1", "Button", 0, path);

            config.SelectPttTwoDevice = Utility.ReadConfigurationString("SELECT_PTT_2", "Device", string.Empty, path);
            config.SelectPttTwoButton = Utility.ReadConfigurationInteger("SELECT_PTT_2", "Button", 0, path);

            config.SelectPttThreeDevice = Utility.ReadConfigurationString("SELECT_PTT_3", "Device", string.Empty, path);
            config.SelectPttThreeButton = Utility.ReadConfigurationInteger("SELECT_PTT_3", "Button", 0, path);

            config.PttCommonDevice = Utility.ReadConfigurationString("COMMON_PTT", "Device", string.Empty, path);
            config.PttCommonButton = Utility.ReadConfigurationInteger("COMMON_PTT", "Button", 0, path);

            config.RadioOnePan = Convert.ToSingle(Utility.ReadConfigurationString("PAN", "Radio1", string.Empty, path), CultureInfo.InvariantCulture);
            config.RadioTwoPan = Convert.ToSingle(Utility.ReadConfigurationString("PAN", "Radio2", string.Empty, path), CultureInfo.InvariantCulture);
            config.RadioThreePan = Convert.ToSingle(Utility.ReadConfigurationString("PAN", "Radio3", string.Empty, path), CultureInfo.InvariantCulture);

            return config;
        }

        /// <summary>
        /// Saves the configuration to file
        /// </summary>
        public void Save()
        {
            var path = Configuration.GetIniFilePath();

            if (!File.Exists(path))
            {
                if (!Directory.Exists(Path.GetDirectoryName(path)))
                {
                    Directory.CreateDirectory(Path.GetDirectoryName(path));
                }

                File.Create(path).Dispose();
            }

            Utility.WriteConfigurationString("General", "TeamSpeakPath", this.TeamSpeakPath, path);

            Utility.WriteConfigurationString("SELECT_PTT_1", "Device", this.SelectPttOneDevice, path);
            Utility.WriteConfigurationString("SELECT_PTT_1", "Button", this.SelectPttOneButton.ToString(), path);

            Utility.WriteConfigurationString("SELECT_PTT_2", "Device", this.SelectPttTwoDevice, path);
            Utility.WriteConfigurationString("SELECT_PTT_2", "Button", this.SelectPttTwoButton.ToString(), path);

            Utility.WriteConfigurationString("SELECT_PTT_3", "Device", this.SelectPttThreeDevice, path);
            Utility.WriteConfigurationString("SELECT_PTT_3", "Button", this.SelectPttThreeButton.ToString(), path);

            Utility.WriteConfigurationString("COMMON_PTT", "Device", this.PttCommonDevice, path);
            Utility.WriteConfigurationString("COMMON_PTT", "Button", this.PttCommonButton.ToString(), path);

            Utility.WriteConfigurationString("PAN", "Radio1", this.RadioOnePan.ToString("F", CultureInfo.InvariantCulture), path);
            Utility.WriteConfigurationString("PAN", "Radio2", this.RadioTwoPan.ToString("F", CultureInfo.InvariantCulture), path);
            Utility.WriteConfigurationString("PAN", "Radio3", this.RadioThreePan.ToString("F", CultureInfo.InvariantCulture), path);
        }

        /// <summary>
        /// Utility method to get the path to the configuration file
        /// </summary>
        /// <returns>The path</returns>
        private static string GetIniFilePath()
        {
            var root = Utility.GetKnownFolderPath(KnownFolderId.LocalAppData);
            return Path.Combine(root, Configuration.FolderName, Configuration.FileName);
        }
    }
}
