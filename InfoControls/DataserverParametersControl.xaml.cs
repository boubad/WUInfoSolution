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
using System.Threading.Tasks;
//
using InfoDomain;
using InfoCouchDB;
//
// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace InfoControls
{
    public sealed partial class DataserverParametersControl : UserControl
    {
        private DatasetEditModel _model = null;
        private bool _busy = false;
        //
        public DataserverParametersControl()
        {
            this.InitializeComponent();
            MyCheckUI().Wait();
        }
        private async Task<bool> Check_params()
        {
            bool bRet = false;
            try
            {
                string surl = textboxBaseUrl.Text.Trim();
                string sdata = textboxDatabaseName.Text.Trim().ToLower();
                bRet = await CouchDBManager.ExistsDatabaseAsync(surl, sdata);
            } catch(Exception /* e */) { }
            return bRet;
        }
        private async void UserControl_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
        {
            _model = null;
            Object obj = this.DataContext;
            if ((obj != null) && (obj is DatasetEditModel))
            {
                _model = obj as DatasetEditModel;
            }
           await  MyCheckUI();
        }
        private async Task MyCheckUI()
        {
            if (_model == null)
            {
                textboxBaseUrl.IsEnabled = false;
                textboxDatabaseName.IsEnabled = false;
                buttonCancel.IsEnabled = false;
                buttonSave.IsEnabled = false;
                return;
            }
            textboxBaseUrl.IsEnabled = true;
            textboxDatabaseName.IsEnabled = true;
            string surl = textboxBaseUrl.Text.Trim();
            string sdata = textboxDatabaseName.Text.Trim().ToLower();
            bool bSave = false;
            bool bCancel = false;
            if ((!string.IsNullOrEmpty(surl)) && (!string.IsNullOrEmpty(sdata)))
            {
                bSave = await Check_params();
                bCancel = (surl != _model.BaseUrl) || (sdata != _model.DatabaseName);
            }
            buttonCancel.IsEnabled = bCancel;
            buttonSave.IsEnabled = bSave;
        }//MyCheckUI
        private  async void textboxBaseUrl_TextChanged(object sender, TextChangedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                _busy = true;
                string surl = textboxBaseUrl.Text.Trim();
                string sdata = textboxDatabaseName.Text.Trim().ToLower();
                if ((!string.IsNullOrEmpty(surl)) && (!string.IsNullOrEmpty(sdata)))
                {
                    await MyCheckUI();
                }
                _busy = false;
            }
        }

        private async void textboxDatabaseName_TextChanged(object sender, TextChangedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                _busy = true;
                string surl = textboxBaseUrl.Text.Trim();
                string sdata = textboxDatabaseName.Text.Trim().ToLower();
                if ((!string.IsNullOrEmpty(surl)) && (!string.IsNullOrEmpty(sdata)))
                {
                    await MyCheckUI();
                }
                _busy = false;
            }
        }

        private async void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                textboxBaseUrl.Text = _model.BaseUrl;
                textboxDatabaseName.Text = _model.DatabaseName;
                await MyCheckUI();
            }
        }

        private void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                string surl = textboxBaseUrl.Text.Trim();
                string sdata = textboxDatabaseName.Text.Trim().ToLower();
                _model.BaseUrl = surl;
                _model.DatabaseName = sdata;
                _busy = false;
            }
        }
    }
}
