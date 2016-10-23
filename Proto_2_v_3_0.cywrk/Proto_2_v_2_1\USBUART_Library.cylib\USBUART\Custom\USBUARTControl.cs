//*******************************************************************************
// File Name: USBUARTControl.cs
/*******************************************************************************
* Copyright 2008-2009, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace USBUART
{
    public partial class USBUARTControl : UserControl
    {
        private USBUARTParameters parameters;

        public USBUARTControl(USBUARTParameters parameters)
        {
            InitializeComponent();
            this.parameters = parameters;
        }

		/*********************************************/
		/* Udate the parameters when GUI is changed: */ 
		/*********************************************/
        private void numUpDownVID_ValueChanged(object sender, EventArgs e)
        {
            parameters.VID = Convert.ToString(numUpDownVID.Value);
        }

        private void numUpDownPID_ValueChanged(object sender, EventArgs e)
        {
            parameters.PID = Convert.ToString(numUpDownPID.Value);
        }

        private void numUpDownRelease_ValueChanged(object sender, EventArgs e)
        {
            parameters.DeviceRelease = Convert.ToString(numUpDownRelease.Value);
        }

        private void textBoxManufacturerString_TextChanged(object sender, EventArgs e)
        {
            parameters.ManufacturerString = textBoxManufacturerString.Text;
        }

        private void textBoxProductString_TextChanged(object sender, EventArgs e)
        {
            parameters.ProductString = textBoxProductString.Text;
        }
    }
}
