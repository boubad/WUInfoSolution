using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236
//
using GestInfoApp.Model;
using GenDomainData;
//
namespace GestInfoApp.Controls
{
    public sealed partial class DatasetUserControl : UserControl
    {
        private StatEditModel m_model;
        private DomainDataset m_old;
        private bool m_busy;
        //
        public DatasetUserControl()
        {
            this.InitializeComponent();
            m_model = null;
            m_old = null;
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
                var p = m_model.CurrentDataset;
                textboxAnnee.Text = p.Annee;
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
                DomainDataset p = null;
                DomainDataset px = null;
                Object obj = listboxItems.SelectedItem;
                if ((obj != null) && (obj is DomainDataset))
                {
                    p = obj as DomainDataset;
                    px = p.Clone();
                }
                m_old = p;
                m_model.CurrentDataset = px;
                fill_item();
                m_model.RefreshVariables();
                m_model.RefreshIndivs();
                m_busy = false;
            }
        }

        private void buttonRefresh_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.RefreshDatasets();
                fill_item();
                m_busy = false;
            }
        }

        private void buttonNew_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentDataset = null;
                fill_item();
                m_busy = false;
            }
        }

        private void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentDataset = m_old;
                fill_item();
                m_busy = false;
            }
        }

        private void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.MaintainsDataset();
                fill_item();
                m_busy = false;
            }
        }

        private void buttonRemove_Click(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.RemoveDataset();
                fill_item();
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
                m_model.CurrentDataset.StatusString = s;
                m_busy = false;
            }
        }

        private void textboxAnnee_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
               m_model.CurrentDataset.Annee = textboxAnnee.Text;
                m_busy = false;
            }
        }
        private void textboxSigle_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
                m_model.CurrentDataset.Sigle = textboxSigle.Text;
                m_busy = false;
            }
        }
        private void textboxName_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
               m_model.CurrentDataset.Name = textboxName.Text;
                m_busy = false;
            }
        }

        private void textboxObservations_LostFocus(object sender, RoutedEventArgs e)
        {
            if ((!m_busy) && (m_model != null))
            {
                m_busy = true;
               m_model.CurrentDataset.Observations = textboxObservations.Text;
                m_busy = false;
            }
        }

        private void UserControl_GotFocus(object sender, RoutedEventArgs e)
        {
            Object obj = listboxItems.SelectedItem;
            if ((obj == null) && (m_model != null))
            {
                var oList = m_model.Datasets;
                if (oList.Count > 0)
                {
                    DomainDataset p = oList.First();
                    listboxItems.SelectedItem = p;
                }
            }
        }
    }
}
