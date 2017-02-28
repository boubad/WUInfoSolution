using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using System.Threading.Tasks;
//
using InfoDomain;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace InfoControls
{
    public sealed partial class DatasetControl : UserControl
    {
        //
        private DatasetEditModel _model;
        private bool _bnew;
        private bool _busy;
        //
        public DatasetControl()
        {
            this.InitializeComponent();
            _model = null;
            _bnew = false;
            _busy = false;
            MyCheckUI();
        }
        //
        private void UserControl_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
        {
            _model = null;
            Object obj = this.DataContext;
            if ((obj != null) && (obj is DatasetEditModel))
            {
                _model = obj as DatasetEditModel;
            }
            MyCheckUI();
        }
        private void MyCheckUI()
        {
            if (_model == null)
            {
                buttonSave.IsEnabled = false;
                buttonRemove.IsEnabled = false;
                buttonCancel.IsEnabled = _bnew;
                buttonNew.IsEnabled = !_bnew;
                buttonRefresh.IsEnabled = false;
                return;
            }
            bool bOk = (!_busy); 
            bool bSave = bOk && _model.IsDatasetStoreable && (_model.Manager != null);
            buttonSave.IsEnabled = bSave;
            bool bRemove = bOk && _model.IsDatasetRemoveable && (!_bnew) && (_model.Manager != null);
            buttonRemove.IsEnabled = bRemove;
            buttonCancel.IsEnabled = bOk && _bnew;
            buttonNew.IsEnabled = bOk && !_bnew;
            buttonRefresh.IsEnabled = bOk && (_model.Manager != null) && (!_bnew);
        }// MyCheckUI
        //
        private void buttonRefresh_Click(object sender, RoutedEventArgs e)
        {
            if (_model != null)
            {
                _model.RefreshDatasets().Wait();
                _bnew = false;
                MyCheckUI();
            }
        }

        private void buttonNew_Click(object sender, RoutedEventArgs e)
        {
            if (_model != null)
            {
                _bnew = true;
                _model.PerformDatasetNew();
                MyCheckUI();
            }
        }

        private void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            if (_model != null)
            {
                _model.PerformDatasetCancel();
                _bnew = false;
                MyCheckUI();
            }
        }

        private void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            if (_model != null)
            {
                _model.PerformDatasetSave().Wait();
                MyCheckUI();
            }
        }

        private void buttonRemove_Click(object sender, RoutedEventArgs e)
        {
            if (_model != null)
            {
                _model.PerformRemoveDataset().Wait();
                MyCheckUI();
            }
        }
        private void listboxSets_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                _busy = true;
                MyCheckUI();
                Dataset p = null;
                Object obj = listboxSets.SelectedItem;
                if ((obj != null) && (obj is Dataset))
                {
                    p = obj as Dataset;
                }
                _model.SelectDataset(p);
                _bnew = false;
                _busy = false;
                MyCheckUI();
            }// not nusy
        }

        private void textboxSigle_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (!_busy)
            {
                MyCheckUI();
            }
        }

        private void textboxName_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (!_busy)
            {
                MyCheckUI();
            }
        }

        private void comboboxStatus_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (!_busy)
            {
                MyCheckUI();
            }
        }
    }
}
