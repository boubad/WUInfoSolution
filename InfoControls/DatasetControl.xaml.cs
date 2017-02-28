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
            MyCheckUI();
        }
        //
        private async void UserControl_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
        {
            _model = null;
            Object obj = this.DataContext;
            if ((obj != null) && (obj is DatasetEditModel))
            {
                _model = obj as DatasetEditModel;
                await _model.RefreshDatasetsAsync();
            }
            MyCheckUI();
        }
        private void MyCheckUI()
        {
            if (_model == null)
            {
                buttonSave.IsEnabled = false;
                buttonRemove.IsEnabled = false;
                buttonCancel.IsEnabled = false;
                buttonNew.IsEnabled = false;
                buttonRefresh.IsEnabled = false;
                return;
            }
            bool bOk = (!_busy); 
            bool bSave = bOk && _model.IsDatasetStoreable;
            buttonSave.IsEnabled = bSave;
            bool bRemove = bOk && _model.IsDatasetRemoveable;
            buttonRemove.IsEnabled = bRemove;
            buttonCancel.IsEnabled = bOk && _model.IsDatasetCancellable;
            buttonNew.IsEnabled = bOk && _model.IsDatasetCreatable;
            buttonRefresh.IsEnabled = bOk && (_model.Manager != null);
        }// MyCheckUI
        //
        private async void buttonRefresh_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    await _model.RefreshDatasetsAsync();
                } catch(Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private async void buttonNew_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    await _model.PerformDatasetNewAsync();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private async void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    await _model.PerformDatasetCancelAsync();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private async void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    await _model.PerformIndivCancelAsync();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private async void buttonRemove_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    await _model.PerformDatasetRemoveAsync();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }
        private async void listboxSets_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    Dataset p = null;
                    Object obj = listboxSets.SelectedItem;
                    if ((obj != null) && (obj is Dataset))
                    {
                        p = obj as Dataset;
                    }
                    await _model.SelectDatasetAsync(p);
                } catch(Exception /* e */) { }
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
