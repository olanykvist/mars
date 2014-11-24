//-----------------------------------------------------------------------
// <copyright file="InputAssignment.cs" company="Master Arms">
//     Copyright (c) Master Arms. All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace MARS.ControlPanel
{
    /// <summary>
    /// POCO class for handling input assignments
    /// </summary>
    internal class InputAssignment
    {
        /// <summary>
        /// Gets or sets the name of the assignment
        /// </summary>
        public string AssignmentName { get; set; }

        /// <summary>
        /// Gets or sets the device name
        /// </summary>
        public string Device { get; set; }

        /// <summary>
        /// Gets or sets the button index
        /// </summary>
        public int Button { get; set; }
    }
}
