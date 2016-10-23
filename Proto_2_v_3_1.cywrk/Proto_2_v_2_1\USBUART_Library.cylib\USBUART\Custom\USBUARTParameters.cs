//*******************************************************************************
// File Name: USBUARTParameters.cs
/*******************************************************************************
* Copyright 2008-2009, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using CyDesigner.Extensions.Gde;

namespace USBUART
{
    public class USBUARTParameters
    {
        private readonly ICyInstEdit_v1 inst;
        public USBUARTControl control;

        private string vid;
        private string pid;
        private string devicerelease;
        private string manufacturerstring;
        private string productstring;

		/* VID Property */
        public string VID
        {
            get { return vid; }
            set
            {
                if (inst != null)
                {
                    vid = value;
                    inst.SetParamExpr("VID", vid.ToString());
                    CommitParams();
                }
            }
        }

		/* PID Property */
        public string PID
        {
            get { return pid; }
            set
            {
                if (inst != null)
                {
                    pid = value;
                    inst.SetParamExpr("PID", pid.ToString());
                    CommitParams();
                }
            }
        }

		/* Device Release Property */
        public string DeviceRelease
        {
            get { return devicerelease; }
            set
            {
                if (inst != null)
                {
                    devicerelease = value;
                    inst.SetParamExpr("DeviceRelease", devicerelease.ToString());
                    CommitParams();
                }
            }
        }

		/* Manufacturer String Property */
        public string ManufacturerString
        {
            get { return manufacturerstring; }
            set
            {
                if (inst != null)
                {
                    manufacturerstring = value;
                    inst.SetParamExpr("ManufacturerString", manufacturerstring.ToString());
                    CommitParams();
                }
            }
        }

		/* Product String Property */
        public string ProductString
        {
            get { return productstring; }
            set
            {
                if (inst != null)
                {
                    productstring = value;
                    inst.SetParamExpr("ProductString", productstring.ToString());
                    CommitParams();
                }
            }
        }

        public USBUARTParameters(ICyInstEdit_v1 inst)
        {
            this.inst = inst;
        }

		/* Get the parameter values from the component and update the GUI: */
        public void GetParams()
        {
            if (inst != null)
            {
                vid = Convert.ToString(inst.GetCommittedParam("VID").Value);
                control.numUpDownVID.Value = Convert.ToDecimal(vid);

                pid = Convert.ToString(inst.GetCommittedParam("PID").Value);
                control.numUpDownPID.Value = Convert.ToDecimal(pid);

                devicerelease = Convert.ToString(inst.GetCommittedParam("DeviceRelease").Value);
                control.numUpDownRelease.Value = Convert.ToDecimal(devicerelease);

                manufacturerstring = Convert.ToString(inst.GetCommittedParam("ManufacturerString").Value);
                control.textBoxManufacturerString.Text = manufacturerstring;

                productstring = Convert.ToString(inst.GetCommittedParam("ProductString").Value);
                control.textBoxProductString.Text = productstring;
            }
        }

		/* Store / Commit the parameters to the GUI: */ 
        public void CommitParams()
        {
            if (inst != null)
            {
                if (!inst.CommitParamExprs())
                {
                    MessageBox.Show("Error in Commiting Parameters");
                }
            }
        }
    }
}