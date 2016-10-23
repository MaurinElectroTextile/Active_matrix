//*******************************************************************************
// File Name: USBUARTCustomizer.cs
/*******************************************************************************
* Copyright 2008-2009, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows.Forms;
using CyDesigner.Extensions.Common;
using CyDesigner.Extensions.Gde;

namespace USBUART
{
    [CyCompDevCustomizer()]
    public partial class CyCustomizer : ICyParamEditHook_v1
    {
        private const bool editParamsOnDrop = false;
        private const CyCompDevParamEditorMode mode = CyCompDevParamEditorMode.COMPLETE;
        private USBUARTParameters parameters;

        DialogResult ICyParamEditHook_v1.EditParams(ICyInstEdit_v1 edit, ICyTerminalQuery_v1 termQuery, ICyExpressMgr_v1 mgr)
        {
            parameters = new USBUARTParameters(edit);  //Declare parameters instance for component

            CyParamExprDelegate paramChange = delegate(ICyParamEditor custEditor, CyCompDevParam param)
            {
                parameters.GetParams();
            };

			/* Add Tabs: */ 
            ICyTabbedParamEditor editor = edit.CreateTabbedParamEditor();
            editor.AddCustomPage("Properties", new USBUARTEditingControl(parameters), paramChange, "Properties");
            editor.AddDefaultPage("Built-in", "Built-in");

            parameters.GetParams();  //Update GUI

            return editor.ShowDialog();
        }

        bool ICyParamEditHook_v1.EditParamsOnDrop
        {
            get
            {
                return editParamsOnDrop;
            }
        }

        CyCompDevParamEditorMode ICyParamEditHook_v1.GetEditorMode()
        {
            return mode;
        }
    }

    public class USBUARTEditingControl : ICyParamEditingControl
    {
        private USBUARTControl control;

        public USBUARTEditingControl(USBUARTParameters parameters)
        {
            control = new USBUARTControl(parameters);
            parameters.control = control;
            control.Dock = DockStyle.Fill;
        }

        Control ICyParamEditingControl.DisplayControl
        {
            get { return control; }
        }

        IEnumerable<CyCustErr> ICyParamEditingControl.GetErrors()
        {
            return new CyCustErr[] { };
        }
    }
}