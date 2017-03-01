using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
//
using InfoDomain;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace InfoControls
{
    public sealed partial class IndivControl : UserControl
    {
        //
        private DatasetEditModel _model = null;
        private bool _busy = false;
        //
        public IndivControl()
        {
            this.InitializeComponent();
            MyCheckUI();
        }

        private  void UserControl_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
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
                buttonCancel.IsEnabled = false;
                buttonNew.IsEnabled = false;
                buttonRefresh.IsEnabled = false;
                return;
            }
            bool bOk = (!_busy);
            bool bSave = bOk && _model.IsIndivStoreable;
            buttonSave.IsEnabled = bSave;
            bool bRemove = bOk && _model.IsIndivRemoveable;
            buttonRemove.IsEnabled = bRemove;
            buttonCancel.IsEnabled = bOk && _model.IsIndivCancellable;
            buttonNew.IsEnabled = bOk && _model.IsIndivCreatable;
            buttonRefresh.IsEnabled = bOk && (_model.Manager != null);
        }// MyCheckUI
        private void listboxIndivs_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    Indiv p = null;
                    Object obj = listboxIndivs.SelectedItem;
                    if ((obj != null) && (obj is Indiv))
                    {
                        p = obj as Indiv;
                    }
                   _model.SelectIndiv(p);
                } catch(Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }// not nusy
        }

        private  void buttonRefresh_Click(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    _model.RefreshIndivs();
                } catch(Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private void buttonNew_Click(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    _model.PerformIndivNew();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }
        private void comboboxStatus_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                var obj = comboboxStatus.SelectedItem;
                String s = (obj == null) ? "Inconnu" : obj.ToString();
                _model.IndivStatus = s;
                _busy = false;
                MyCheckUI();
            }
        }

        private void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    _model.PerformIndivCancel();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    _model.PerformIndivSave();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private void buttonRemove_Click(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    _model.PerformIndivRemove();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private void textboxSigle_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                _model.IndivSigle = textboxSigle.Text;
                _busy = false;
                MyCheckUI();
            }
        }

        private void textboxName_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                _model.IndivName = textboxName.Text;
                _busy = false;
                MyCheckUI();
            }
        }

        private void textboxObservations_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                _model.IndivObservations = textboxObservations.Text;
                _busy = false;
                MyCheckUI();
            }
        }
    }
}
