//-----------------------------------------------------------------------
// <copyright file="Configuration.cs" company="Master Arms">
//     Copyright (c) Master Arms. All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace MARS.Common
{
    using System;
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
        /// Loads current configuration from file
        /// </summary>
        /// <returns>The current configuration</returns>
        public static Configuration Load()
        {
            var config = new Configuration();
            var path = Configuration.GetIniFilePath();
            config.TeamSpeakPath = Utility.ReadConfigurationString("General", "TeamSpeakPath", string.Empty, path);
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
