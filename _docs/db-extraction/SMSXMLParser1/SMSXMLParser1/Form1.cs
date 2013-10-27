using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Xml;

namespace SMSXMLParser1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            textBox1.Multiline = true;
            textBox1.WordWrap = false;
            textBox1.Anchor = AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom | AnchorStyles.Left;
            textBox1.ScrollBars = ScrollBars.Vertical;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            StringBuilder csv = new StringBuilder();

            string sub_category_id = string.Empty;
            string sub_category_title = string.Empty;
            string category_id = string.Empty;
            string category_title = string.Empty;
            string seri_title = string.Empty;
            string seri_uri = string.Empty;
            string sms_rate = string.Empty;
            string sms_id = string.Empty;
            string sms = string.Empty;

            String xmlString = string.Empty;

            using (StreamReader sr = new StreamReader(@"xml-sms\groups.xml", Encoding.UTF8, true))
            {
                xmlString = sr.ReadToEnd();
                sr.Close();
            }

            if (xmlString == string.Empty)
                return;

            // Create an XmlReader
            using (XmlReader reader = XmlReader.Create(new StringReader(xmlString)))
            {
                while (reader.Read())
                {
                    if (reader.Name == "sub_category")
                    {
                        if (reader.IsStartElement())
                        {
                            sub_category_id = reader.GetAttribute("id");
                            sub_category_title = reader.GetAttribute("title");
                        }
                    }
                    else if (reader.Name == "category")
                    {
                        if (reader.IsStartElement())
                        {
                            category_id = reader.GetAttribute("id");
                            category_title = reader.GetAttribute("title");
                        }
                    }
                    else if (reader.Name == "seri")
                    {
                        seri_title = reader.GetAttribute("title");
                        seri_uri = reader.GetAttribute("uri");

                        String seri_xmlString = string.Empty;

                        using (StreamReader sr = new StreamReader(string.Format(@"xml-sms\{0}", seri_uri), Encoding.UTF8, true))
                        {
                            seri_xmlString = sr.ReadToEnd();
                            sr.Close();
                        }

                        using (XmlReader seri_reader = XmlReader.Create(new StringReader(seri_xmlString)))
                        {
                            while (seri_reader.Read())
                            {
                                if (seri_reader.Name == "sms")
                                {
                                    if (seri_reader.IsStartElement())
                                    {
                                        sms_rate = seri_reader.GetAttribute("rate");
                                        sms_id = seri_reader.GetAttribute("id");
                                        sms = seri_reader.ReadInnerXml();

                                        csv.AppendLine(string.Format("\"{0}\",\"{1}\"",
                                            string.Format("{0} - {1} - {2}",
                                            sub_category_title.Replace("\"", "\"\""),
                                            category_title.Replace("\"", "\"\""),
                                            seri_title.Replace("\"", "\"\"")),
                                            sms.Replace("\"", "\"\"")));
                                    }
                                }
                            }
                        }
                    }
                }
            }

            textBox1.Text = csv.ToString();
        }
    }
}
