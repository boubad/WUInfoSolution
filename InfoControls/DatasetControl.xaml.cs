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
        private DatasetEditModel _model = null;
        private bool _busy = false;
        //
        public DatasetControl()
        {
            this.InitializeComponent();
            if (comboboxStatus != null)
            {
                var oList = Dataset.AllStatusStrings();
                if (oList != null)
                {
                    foreach (var p in oList)
                    {
                        comboboxStatus.Items.Add(p);
                    }// p
                }// oList
            }
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
        }
        //
        private void buttonRefresh_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                _model.RefreshDatasets();
                _busy = false;
            }
        }

        private void buttonNew_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                _model.PerformDatasetNew();
                _busy = false;
            }
        }

        private void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                _model.PerformDatasetCancel();
                _busy = false;
            }
        }

        private void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                _model.PerformDatasetSave();
                _busy = false;
            }
        }

        private void buttonRemove_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                _model.PerformDatasetRemove();
                _busy = false;
            }
        }
        private void listboxSets_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                _busy = true;
                Dataset p = null;
                Object obj = listboxSets.SelectedItem;
                if ((obj != null) && (obj is Dataset))
                {
                    p = obj as Dataset;
                }
                _model.SelectDataset(p);
                _busy = false;
            }// not nusy
        }
    }
}
