using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
//
using GenDomainData;
using GestInfoApp.Controls;
using GestInfoApp.Model;
//
// Pour plus d'informations sur le modèle d'élément Page vierge, consultez la page https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace GestInfoApp
{
    /// <summary>
    /// Une page vide peut être utilisée seule ou constituer une page de destination au sein d'un frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private StatEditModel m_editmodel;
        //
        public MainPage()
        {
            this.InitializeComponent();
        }
        public StatEditModel EditModel
        {
            get
            {
                if( m_editmodel == null)
                {
                    var localSettings = Windows.Storage.ApplicationData.Current.LocalSettings;
                    string url = localSettings.Values.ContainsKey("BaseUrl") ? localSettings.Values["BaseUrl"].ToString() : null;
                    if (string.IsNullOrEmpty(url))
                    {
                        url = "http://localhost:5984/";
                    }
                    string database = localSettings.Values.ContainsKey("DatabaseName") ? localSettings.Values["DatabaseName"].ToString() : null;
                    if (string.IsNullOrEmpty(database))
                    {
                        database = "test";
                    }
                    m_editmodel = new StatEditModel(url, database);
                }// model
                return m_editmodel;
            }
            set
            {
                if (m_editmodel != value)
                {
                    m_editmodel = value;
                }
            }
        }// EditModel

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            DataContext = EditModel;
        }
    }
}
