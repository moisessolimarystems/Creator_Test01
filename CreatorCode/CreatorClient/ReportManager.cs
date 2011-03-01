using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.AccessControl;
using System.IO;
using System.Xml;
using Microsoft.Win32;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    class ReportManager
    {
        private CommunicationLink _commLink;

        public ReportManager(CommunicationLink commLink)
        {
            _commLink = commLink;
        }

        public void DeleteReport(Report report)
        {
            int findIndex;
            List<Report> reports = LoadReportsFromXML();
            findIndex = reports.FindIndex(r => r.ID == report.ID);
            if (findIndex >= 0)
                reports.RemoveAt(findIndex);
            SaveReportsToXML(reports);
        }

        public void SaveReport(Report report)
        {
            int findIndex;
            List<Report> reports = LoadReportsFromXML();
            findIndex = reports.FindIndex(r => r.ID == report.ID);
            if (findIndex >= 0)
                reports.RemoveAt(findIndex);            
            reports.Add(report);
            SaveReportsToXML(reports);
        }

        public List<Report> GetReports()
        {
            return LoadReportsFromXML();
        }

        /*
          * Function    : AddFileSecurity
          * Description : Gives account the supplied rights to filename
          * Parameters  : fileName - 
          *				 account  - 
          *				 rights	  -
          *				 controlType -
          */
        private void AddFileSecurity(String fileName, String account,
                                FileSystemRights rights, AccessControlType controlType)
        {
            // Get a FileSecurity object that represents the 
            // current security settings.
            FileSecurity fSecurity = File.GetAccessControl(fileName);

            // Add the FileSystemAccessRule to the security settings. 
            fSecurity.AddAccessRule(new FileSystemAccessRule
                                           (account, rights, controlType));

            // Set the new access settings.
            File.SetAccessControl(fileName, fSecurity);
        }
        
        #region Save/Load from XML
        private List<Report> LoadReportsFromXML()
        {           
            List<Report> reportList = new List<Report>();
            String filePath = String.Concat(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData),
                                            AppConstants.LocalConnectionFilePath,
                                            AppConstants.LocalConnectionFile);
            if (File.Exists(filePath))
            {
                FileStream fs = new FileStream(filePath, FileMode.OpenOrCreate, FileAccess.Read, FileShare.Read);
                XmlReader reader = XmlReader.Create(fs);    // Create the XmlReader object.

                //find product section for specified id in xml
                try
                {
                    bool bXmlNodeFound = false;
                    string tmpString = null;
                    do
                    {
                        bXmlNodeFound = reader.ReadToFollowing(AppConstants.ReportXMLTags.ReportElement);
                        if (!bXmlNodeFound)
                            continue;
                        tmpString = reader.GetAttribute(AppConstants.ReportXMLTags.IdAttrib);
                        if (tmpString == null)
                            continue;
                        Report newReport = new Report(_commLink);
                        newReport.ID = tmpString;
                        tmpString = reader.GetAttribute(AppConstants.ReportXMLTags.TypeAttrib);
                        if (tmpString == null)
                            continue;
                        newReport.Type = (Report.ReportType)Enum.Parse(typeof(Report.ReportType), tmpString);
                        tmpString = reader.GetAttribute(AppConstants.ReportXMLTags.MatchAttrib);
                        if (tmpString == null)
                            newReport.MatchAll = true;
                        else
                            newReport.MatchAll = (tmpString == bool.TrueString) ? true : false;
                        reader.ReadToFollowing(AppConstants.ReportXMLTags.ConditionElement);
                        if (!bXmlNodeFound)
                            continue;
                        do
                        {
                            Condition newCondition = new Condition();
                            tmpString = reader.GetAttribute(AppConstants.ReportXMLTags.NameAttrib);
                            if (tmpString == null)
                                continue;
                            newCondition.Name = (CreatorService.ConditionName)Enum.Parse(typeof(CreatorService.ConditionName), tmpString);
                            tmpString = reader.GetAttribute(AppConstants.ReportXMLTags.OperatorAttrib);
                            if (tmpString == null)
                                continue;
                            newCondition.Operator = (CreatorService.ConditionOperator)Enum.Parse(typeof(CreatorService.ConditionOperator), tmpString);
                            tmpString = reader.GetAttribute(AppConstants.ReportXMLTags.ValueAttrib);
                            if (tmpString == null)
                                continue;
                            newCondition.Value = tmpString;
                            tmpString = reader.GetAttribute(AppConstants.ReportXMLTags.TypeAttrib);
                            if (tmpString == null)
                                tmpString = "";
                            newCondition.ValueType = tmpString;

                            newReport.Conditions.Add(newCondition);
                        } while (reader.ReadToNextSibling(AppConstants.ReportXMLTags.ConditionElement));
                        reportList.Add(newReport);
                    } while (bXmlNodeFound);
                }
                catch (Exception)
                {
                }
                reader.Close();
                fs.Close();
            }
            return reportList;
        }

        //returns false if there is a problem saving to XML
        private bool SaveReportsToXML(List<Report> reportList)
        {
            bool bRetVal = false;
            String path = String.Concat(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), AppConstants.LocalConnectionFilePath, "");
            DirectoryInfo di = new DirectoryInfo(path);
            try
            {
                if (di.Exists == false)
                    di.Create();
            }
            catch (Exception)
            {
                throw;
            }
            String filePath = String.Concat(path, AppConstants.LocalConnectionFile);
            String tempFilePath = String.Concat(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData),
                                                AppConstants.LocalConnectionFilePath,
                                                "temp.xml");
            FileStream fsWrite = new FileStream(tempFilePath, FileMode.OpenOrCreate, FileSystemRights.Modify, FileShare.ReadWrite, 8, FileOptions.None);
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.Indent = true;
            XmlWriter writer = XmlWriter.Create(fsWrite, settings);

            writer.WriteStartElement(AppConstants.ReportXMLTags.ReportStructureElement);  //ReportStructureElement
            if (reportList.Count > 0)
            {
                foreach (Report rp in reportList)
                {
                    writer.WriteStartElement(AppConstants.ReportXMLTags.ReportElement);   //Report Element
                    writer.WriteAttributeString(AppConstants.ReportXMLTags.IdAttrib, rp.ID);
                    writer.WriteAttributeString(AppConstants.ReportXMLTags.TypeAttrib, rp.Type.ToString());
                    writer.WriteAttributeString(AppConstants.ReportXMLTags.MatchAttrib, rp.MatchAll.ToString());
                    foreach (Condition c in rp.Conditions)
                    {
                        writer.WriteStartElement(AppConstants.ReportXMLTags.ConditionElement);    //Condition Element
                        writer.WriteAttributeString(AppConstants.ReportXMLTags.NameAttrib, c.Name.ToString());
                        writer.WriteAttributeString(AppConstants.ReportXMLTags.OperatorAttrib, c.Operator.ToString());
                        writer.WriteAttributeString(AppConstants.ReportXMLTags.ValueAttrib, c.Value);
                        if(c.ValueType != null)
                            if(c.ValueType.Length > 0)
                                writer.WriteAttributeString(AppConstants.ReportXMLTags.TypeAttrib, c.ValueType);
                        writer.WriteEndElement();       //Close Condition Element
                    }
                    writer.WriteFullEndElement();   //Close Report Element
                }
            }
            writer.WriteFullEndElement();   //Close ReportStructureElement Element

            writer.Flush();
            writer.Close();
            fsWrite.Close();

            File.Delete(filePath);
            File.Move(tempFilePath, filePath);
            try
            {
                //give everyone rights to new file.
                //CR.FIX.11396 - For other than english OS, there is no everyone group, this will find the correct everyone group based on os language
                System.Security.Principal.SecurityIdentifier secId = new System.Security.Principal.SecurityIdentifier(System.Security.Principal.WellKnownSidType.WorldSid, null);
                System.Security.Principal.NTAccount acct = (System.Security.Principal.NTAccount)(secId.Translate(typeof(System.Security.Principal.NTAccount)));
                String strEveryoneAccount = acct.ToString();
                AddFileSecurity(filePath,
                                strEveryoneAccount,
                                FileSystemRights.FullControl,
                                AccessControlType.Allow);
            }
            catch (Exception)
            {
            }
            bRetVal = true;
            return bRetVal;
        }
        #endregion
    }
}
