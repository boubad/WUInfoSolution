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
        private DatasetEditModel _model;
        private bool _bnew;
        private bool _busy;
        //
        public VariableControl()
        {
            this.InitializeComponent();
            _model = null;
            _bnew = false;
            _busy = false;
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
                buttonCancel.IsEnabled = _bnew;
                buttonNew.IsEnabled = !_bnew;
                buttonRefresh.IsEnabled = false;
                return;
            }
            bool bOk = (!_busy);
            bool bSave = bOk && _model.IsVariableStoreable && (_model.Manager != null);
            buttonSave.IsEnabled = bSave;
            bool bRemove = bOk && _model.IsVariableRemoveable && (!_bnew) && (_model.Manager != null);
            buttonRemove.IsEnabled = bRemove;
            buttonCancel.IsEnabled = bOk && _bnew;
            buttonNew.IsEnabled = bOk && !_bnew;
            buttonRefresh.IsEnabled = bOk && (_model.Manager != null) && (!_bnew);
        }// MyCheckUI

        private void listboxVariables_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!_busy) && (_model != null))
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
                _bnew = false;
                _busy = false;
                MyCheckUI();
            }// not nusy
        }

        private void buttonRefresh_Click(object sender, RoutedEventArgs e)
        {
            if (_model != null)
            {
                _model.RefreshVariables().Wait();
                MyCheckUI();
            }
        }

        private void buttonNew_Click(object sender, RoutedEventArgs e)
        {
            if (_model != null)
            {
                _bnew = true;
                _model.PerformVariableNew();
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

        private void comboboxType_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (!_busy)
            {
                MyCheckUI();
            }
        }

        private void comboboxKind_SelectionChanged(object sender, SelectionChangedEventArgs e)
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

        private void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            if (_model != null)
            {
                _model.PerformVariableCancel();
                _bnew = false;
                MyCheckUI();
            }
        }

        private void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            if (_model != null)
            {
                _model.PerformVariableSave().Wait();
                MyCheckUI();
            }
        }

        private void buttonRemove_Click(object sender, RoutedEventArgs e)
        {
            if (_model != null)
            {
                _model.PerformVariableRemove().Wait();
                MyCheckUI();
            }
        }
    }
}
