using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using System.Linq;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236
using GestInfoApp.Model;
using GenDomainData;
//
namespace GestInfoApp.Controls
{
    public sealed partial class IndivUserControl : UserControl
    {
        private StatEditModel m_model;
        private DomainIndiv m_old;
        private DomainValue m_oldval;
        private bool m_busy;
        //
        public IndivUserControl()
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
                var p = m_model.CurrentIndiv;
                textboxSigle.Text = p.Sigle;
                textboxName.Text = p.Name;
                textboxObservations.Text = p.Observations;
                comboboxStatus.SelectedItem = p.StatusString;
            }// m_model
        }// fillItem
        private void listboxItems_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                DomainIndiv p = null;
                DomainIndiv px = null;
                Object obj = listboxItems.SelectedItem;
                if ((obj != null) && (obj is DomainIndiv))
                {
                    p = obj as DomainIndiv;
                    px = p.Clone();
                }
                m_old = p;
                m_model.CurrentIndiv = px;
                fill_item();
                m_model.RefreshIndivValues();
                m_busy = false;
            }
        }

        private void buttonRefresh_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.RefreshIndivs();
                fill_item();
                m_busy = false;
            }
        }

        private void buttonNew_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentIndiv = null;
                fill_item();
                m_busy = false;
            }
        }

        private void textboxSigle_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentIndiv.Sigle = textboxSigle.Text;
                m_busy = false;
            }
        }

        private void textboxName_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentIndiv.Name = textboxName.Text;
                m_busy = false;
            }
        }

        private void textboxObservations_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentIndiv.Observations = textboxObservations.Text;
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
                m_model.CurrentIndiv.StatusString = s;
                m_busy = false;
            }
        }

        private void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentIndiv = m_old;
                fill_item();
                m_busy = false;
            }
        }

        private void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.MaintainsIndiv();
                fill_item();
                m_busy = false;
            }
        }
        private void buttonRemove_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.RemoveIndiv();
                fill_item();
                m_busy = false;
            }
        }

        private void UserControl_GotFocus(object sender, RoutedEventArgs e)
        {
            Object obj = listboxItems.SelectedItem;
            if ((obj == null) && (m_model != null))
            {
                var oList = m_model.Indivs;
                if (oList.Count > 0)
                {
                    DomainIndiv p = oList.First();
                    listboxItems.SelectedItem = p;
                }
            }
        }

        private void listboxVariables_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                DomainValue p = null;
                DomainValue px = null;
                Object obj = listboxVariables.SelectedItem;
                if ((obj != null) && (obj is DomainValue))
                {
                    p = obj as DomainValue;
                    px = p.Clone();
                }
                m_oldval = p;
                m_model.CurrentIndivValue = px;
                textboxValue.Text = m_model.CurrentIndivValue.StringValue;
                m_busy = false;
            }
        }

        private void textboxValue_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentIndivValue.StringValue = textboxValue.Text;
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
                    DomainValue px = m_model.CurrentIndivValue;
                    var col = m_model.IndivValues.Where(x => (x.VariableSigle == px.VariableSigle) && (x.IndivSigle == px.IndivSigle)).ToList();
                    if (col.Count > 0)
                    {
                        p = col.First();
                    }
                    if (p != null)
                    {
                        p.StringValue = sval;
                        var old = m_model.IndivValues;
                        m_model.IndivValues = null;
                        m_model.IndivValues = old;
                        m_model.IsIndivValuesModified = true;
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
                m_model.CurrentIndivValue = m_oldval;
                m_busy = false;
            }
        }

        private void buttonAllSave_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.MaintainsIndivValues();
                m_busy = false;
            }
        }
    }
}
