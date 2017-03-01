using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
//
using InfoDomain;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace InfoControls
{
    public sealed partial class VariableControl : UserControl
    {
        //
        private DatasetEditModel _model = null;
        private bool _busy = false;
        //
        public VariableControl()
        {
            this.InitializeComponent();
            MyCheckUI();
        }
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
                buttonCancel.IsEnabled = false;
                buttonNew.IsEnabled = false;
                buttonRefresh.IsEnabled = false;
                return;
            }
            bool bOk = (!_busy);
            bool bSave = bOk && _model.IsVariableStoreable;
            buttonSave.IsEnabled = bSave;
            bool bRemove = bOk && _model.IsVariableRemoveable;
            buttonRemove.IsEnabled = bRemove;
            buttonCancel.IsEnabled = bOk && _model.IsVariableCancellable;
            buttonNew.IsEnabled = bOk && _model.IsVariableCreatable;
            buttonRefresh.IsEnabled = bOk && (_model.Manager != null);
        }// MyCheckUI

        private void listboxVariables_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    Variable p = null;
                    Object obj = listboxVariables.SelectedItem;
                    if ((obj != null) && (obj is Variable))
                    {
                        p = obj as Variable;
                    }
                    _model.SelectVariable(p);
                } catch(Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }// not nusy
        }

        private void buttonRefresh_Click(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                try
                {
                    _busy = true;
                    MyCheckUI();
                    _model.RefreshVariables();
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
                     _model.PerformVariableNew();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }
        private void comboboxType_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                var obj = comboboxType.SelectedItem;
                String s = (obj == null) ? "Inconnu" : obj.ToString();
                _model.VariableType = s;
                _busy = false;
                MyCheckUI();
            }
        }

        private void comboboxKind_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((_model != null) && (!_busy))
            {
                _busy = true;
                var obj = comboboxKind.SelectedItem;
                String s = (obj == null) ? "Inconnu" : obj.ToString();
                _model.VariableKind = s;
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
                _model.VariableStatus = s;
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
                    _model.PerformVariableCancel();
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
                    _model.PerformVariableSave();
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
                    _model.PerformVariableRemove();
                }
                catch (Exception /* e */) { }
                _busy = false;
                MyCheckUI();
            }
        }

        private void textboxSigle_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                _busy = true;
                _model.VariableSigle = textboxSigle.Text;
                _busy = false;
                MyCheckUI();
            }
        }

        private void textboxName_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                _busy = true;
                _model.VariableName = textboxName.Text;
                _busy = false;
                MyCheckUI();
            }
        }

        private void textboxObservations_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!_busy) && (_model != null))
            {
                _busy = true;
                _model.VariableObservations = textboxObservations.Text;
                _busy = false;
                MyCheckUI();
            }
        }
    }
}
