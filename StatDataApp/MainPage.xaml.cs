﻿using System;
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
using InfoControls;
using StatDataApp.Data;
// Pour plus d'informations sur le modèle d'élément Page vierge, consultez la page https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace StatDataApp
{
    /// <summary>
    /// Une page vide peut être utilisée seule ou constituer une page de destination au sein d'un frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private DatasetEditModel _editmodel = null;
        private InfoParams _params = null;
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            if (_editmodel == null)
            {
                if (_params == null)
                {
                    _params = new InfoParams();
                }
                _editmodel = new DatasetEditModel(_params.BaseUrl, _params.DatabaseName);
            }// _editmodel
            if (datasetEditControl != null)
            {
                datasetEditControl.DataContext = _editmodel;
            }
        }
    }
}
