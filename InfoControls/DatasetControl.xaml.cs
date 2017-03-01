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
        private void UserControl_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
        {
            _model = null;
            Object obj = this.DataContext;
            if ((obj != null) && (obj is DatasetEditModel))
            {
                _model = obj as DatasetEditModel;
               // _model.RefreshDatasetsAsync().Wait();
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
        private void buttonRefresh_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    _model.RefreshDatasets();
                } catch(Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private void buttonNew_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    _model.PerformDatasetNew();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private  void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    _model.PerformDatasetCancel();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    _model.PerformDatasetSave();
                }
                catch (Exception ex ) {
                    string s = ex.Message;
                    string s3 = ex.ToString();
                    string z = s;
                }
                _busy = false;
                MyCheckUI();
            }
        }

        private void buttonRemove_Click(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    _model.PerformDatasetRemove();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }
        private void listboxSets_SelectionChanged(object sender, SelectionChangedEventArgs e)
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
                    _model.SelectDataset(p);
                } catch(Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }// not nusy
        }
        private void comboboxStatus_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                var obj = comboboxStatus.SelectedItem;
                String s = (obj == null) ? "Inconnu" : obj.ToString();
                _model.DatasetStatus = s;
                _busy = false;
                MyCheckUI();
            }
        }

       

        private void textboxSigle_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                _model.DatasetSigle = textboxSigle.Text;
                 _busy = false;
                MyCheckUI();
            }
        }

        private void textboxName_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                 _busy = true;
                 _model.DatasetName = textboxName.Text;
                 _busy = false;
                MyCheckUI();
            }
        }

        private void textboxAnnee_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                 _busy = true;
                 _model.DatasetAnnee = textboxAnnee.Text;
                 _busy = false;
                MyCheckUI();
            }
        }

        private void textboxObservations_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                 _busy = true;
                 _model.DatasetObservations = textboxObservations.Text;
                  _busy = false;
                MyCheckUI();
            }
        }
    }
}
