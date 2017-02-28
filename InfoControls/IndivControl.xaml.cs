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

        private async void UserControl_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
        {
            _model = null;
            Object obj = this.DataContext;
            if ((obj != null) && (obj is DatasetEditModel))
            {
                _model = obj as DatasetEditModel;
                await _model.RefreshIndivsAsync();
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
        private async void listboxIndivs_SelectionChanged(object sender, SelectionChangedEventArgs e)
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
                   await _model.SelectIndivAsync(p);
                } catch(Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }// not nusy
        }

        private  async void buttonRefresh_Click(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    await _model.RefreshIndivsAsync();
                } catch(Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private async void buttonNew_Click(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    await _model.PerformIndivNewAsync();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
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

        private async void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
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

        private async void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    await _model.PerformIndivSaveAsync();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private async void buttonRemove_Click(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    await _model.PerformIndivRemoveAsync();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }
    }
}
