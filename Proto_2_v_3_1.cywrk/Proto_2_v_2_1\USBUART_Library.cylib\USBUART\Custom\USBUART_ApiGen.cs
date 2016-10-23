//*******************************************************************************
// File Name: USBUART_ApiGen.cs
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
using CyDesigner.Extensions.Common;
using CyDesigner.Extensions.Gde;

namespace USBUART
{
    partial class CyCustomizer : ICyAPICustomize_v1
    {
        #region ParameterNamesValues
        string VID_MSB_ParamName = "VID_MSB";
        string VID_LSB_ParamName = "VID_LSB";
        string VID_ParamName = "VID";
        string PID_MSB_ParamName = "PID_MSB";
        string PID_LSB_ParamName = "PID_LSB";
        string PID_ParamName = "PID";
        string INI_VID_ParamName = "INI_VID";
        string INI_PID_ParamName = "INI_PID";
        string DeviceRelease_ParamName = "DeviceRelease";
        string DeviceRelease_MSB_ParamName = "DeviceRelease_MSB";
        string DeviceRelease_LSB_ParamName = "DeviceRelease_LSB";

        string Temp = "";
        UInt16 xyz;

        #endregion

        ICyTerminalQuery_v1 m_termQuery;
        ICyInstQuery_v1 m_instQuery;

        public IEnumerable<CyAPICustomizer> CustomizeAPIs(ICyInstQuery_v1 query, ICyTerminalQuery_v1 termQuery, IEnumerable<CyAPICustomizer> apis)
        {
            List<CyAPICustomizer> customizers = new List<CyAPICustomizer>(apis);
            Dictionary<string, string> paramDict = null;
            m_termQuery = termQuery;
            m_instQuery = query;

            ICyDesignQuery_v1 desquery = m_instQuery.DesignQuery;

            string tmpString = "";
			
			//Fill Dictionary with parameters:
            foreach (CyAPICustomizer api in customizers)
            {
                paramDict = api.MacroDictionary;
            }
			/**********************/
			/* Modify Parameters: */
			/**********************/
            /* VID: */
            paramDict.TryGetValue(VID_ParamName, out tmpString); //Get VID Value

            xyz = Convert.ToUInt16(tmpString);			//Convert to int
            Temp = String.Format("{0,4:X4}", xyz);	//Convert to hex string

            paramDict.Add(VID_MSB_ParamName, "0x" + Temp.Substring(0, 2) + "u");   //MSB
            paramDict.Add(VID_LSB_ParamName, "0x" + Temp.Substring(2, 2) + "u");	//LSB

            paramDict.Add(INI_VID_ParamName, Temp.Substring(0, 4));		      //INI VID


            /* PID: */
            paramDict.TryGetValue(PID_ParamName, out tmpString); //Get PID Value

            xyz = Convert.ToUInt16(tmpString);			//Convert to int
            Temp = String.Format("{0,4:X4}", xyz);	//Convert to hex string

            paramDict.Add(PID_MSB_ParamName, "0x" + Temp.Substring(0, 2) + "u");   //MSB
            paramDict.Add(PID_LSB_ParamName, "0x" + Temp.Substring(2, 2) + "u");	//LSB

            paramDict.Add(INI_PID_ParamName, Temp.Substring(0, 4));		      //INI PID

            /* Device Release: */
            paramDict.TryGetValue(DeviceRelease_ParamName, out tmpString); //Get Device Release Value

            xyz = Convert.ToUInt16(tmpString);			//Convert to int
            Temp = String.Format("{0,4:X4}", xyz);	    //Convert to hex string

            paramDict.Add(DeviceRelease_MSB_ParamName, "0x" + Temp.Substring(0, 2) + "u");   //MSB
            paramDict.Add(DeviceRelease_LSB_ParamName, "0x" + Temp.Substring(2, 2) + "u");	//LSB	

			//Commit modified parameters:
            foreach (CyAPICustomizer api in apis)
            {
                api.MacroDictionary = paramDict;
            }

            return apis;
        }
    }
}