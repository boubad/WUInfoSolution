using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using System.Linq;
// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236
//
using GestInfoApp.Model;
using GenDomainData;
//
//
namespace GestInfoApp.Controls
{
    public sealed partial class VariableUserControl : UserControl
    {
        private StatEditModel m_model;
        private DomainVariable m_old;
        private DomainValue m_oldval;
        private bool m_busy;
        //
        public VariableUserControl()
        {
            this.InitializeComponent();
            m_model = null;
            m_old = null;
            m_oldval = null;
            m_busy = false;
        }

        private void UserControl_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
        {
            StatEditModel p = null;
            Object obj = DataContext;
            if ((obj != null) && (obj is StatEditModel))
            {
                p = obj as StatEditModel;
            }
            m_model = p;
            fill_item();
        }
        private void fill_item()
        {
            if (m_model != null)
            {
                var p = m_model.CurrentVariable;
                textboxSigle.Text = p.Sigle;
                textboxName.Text = p.Name;
                comboboxType.SelectedItem = p.VariableTypeString;
                comboboxKind.SelectedItem = p.VariableKindString;
                textboxModalites.Text = p.ModalitesString;
                textboxObservations.Text = p.Observations;
                comboboxStatus.SelectedItem = p.StatusString;
            }// m_model
        }// fillItem
        private void listboxItems_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                DomainVariable p = null;
                DomainVariable px = null;
                Object obj = listboxItems.SelectedItem;
                if ((obj != null) && (obj is DomainVariable))
                {
                    p = obj as DomainVariable;
                    px = p.Clone();
                }
                m_old = p;
                m_model.CurrentVariable = px;
                fill_item();
                m_model.RefreshVariableValues();
                m_busy = false;
            }
        }

        private void buttonRefresh_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.RefreshVariables();
                fill_item();
                m_busy = false;
            }
        }

        private void buttonNew_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentVariable = null;
                fill_item();
                m_busy = false;
            }
        }

        private void textboxSigle_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentVariable.Sigle = textboxSigle.Text;
                m_busy = false;
            }
        }

        private void textboxName_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentVariable.Name = textboxName.Text;
                m_busy = false;
            }
        }

        private void comboboxType_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                string s = null;
                Object obj = comboboxType.SelectedItem;
                if (obj != null)
                {
                    s = obj.ToString();
                }
                if (string.IsNullOrEmpty(s))
                {
                    s = "Unknown";
                }
                m_model.CurrentVariable.VariableTypeString = s;
                m_busy = false;
            }
        }

        private void comboboxKind_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                string s = null;
                Object obj = comboboxKind.SelectedItem;
                if (obj != null)
                {
                    s = obj.ToString();
                }
                if (string.IsNullOrEmpty(s))
                {
                    s = "Unknown";
                }
                m_model.CurrentVariable.VariableKindString = s;
                m_busy = false;
            }
        }

        private void textboxModalites_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentVariable.ModalitesString= textboxModalites.Text;
                m_busy = false;
            }
        }

        private void textboxObservations_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentVariable.Observations = textboxObservations.Text;
                m_busy = false;
            }
        }

        private void comboboxStatus_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                string s = null;
                Object obj = comboboxStatus.SelectedItem;
                if (obj != null)
                {
                    s = obj.ToString();
                }
                if (string.IsNullOrEmpty(s))
                {
                    s = "Unknown";
                }
                m_model.CurrentVariable.StatusString = s;
                m_busy = false;
            }
        }

        private void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentVariable = m_old;
                fill_item();
                m_busy = false;
            }
        }

        private void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.MaintainsVariable();
                fill_item();
                m_busy = false;
            }
        }

        private void buttonRemove_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.RemoveVariable();
                fill_item();
                m_busy = false;
            }
        }
        private void UserControl_GotFocus(object sender, RoutedEventArgs e)
        {
            Object obj = listboxItems.SelectedItem;
            if ((obj == null) && (m_model != null))
            {
                var oList = m_model.Variables;
                if (oList.Count > 0)
                {
                    DomainVariable p = oList.First();
                    listboxItems.SelectedItem = p;
                }
            }
        }

        private void listboxIndivs_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                DomainValue p = null;
                DomainValue px = null;
                Object obj = listboxIndivs.SelectedItem;
                if ((obj != null) && (obj is DomainValue))
                {
                    p = obj as DomainValue;
                    px = p.Clone();
                }
                m_oldval = p;
                m_model.CurrentVariableValue = px;
                textboxValue.Text = m_model.CurrentVariableValue.StringValue;
                m_busy = false;
            }
        }

        private void textboxValue_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentVariableValue.StringValue = textboxValue.Text;
                m_busy = false;
            }
        }

        private void buttonDetailSave_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                if (m_oldval != null)
                {
                    string sval = textboxValue.Text;
                    DomainValue p = null;
                    DomainValue px = m_model.CurrentVariableValue;
                    var col = m_model.VariableValues.Where(x => (x.VariableSigle == px.VariableSigle) && (x.IndivSigle == px.IndivSigle)).ToList();
                    if (col.Count > 0)
                    {
                        p = col.First();
                    }
                    if (p != null)
                    {
                        p.StringValue = sval;
                        var old = m_model.VariableValues;
                        m_model.VariableValues = null;
                        m_model.VariableValues = old;
                        m_model.IsVariableValuesModified = true;
                    }
                }
                m_busy = false;
            }
        }

        private void buttonDetailCancel_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentVariableValue = m_oldval;
                m_busy = false;
            }
        }

        private void buttonAllSave_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.MaintainsVariableValues();
                m_busy = false;
            }
        }
    }
}
