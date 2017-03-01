using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatDataApp.Data
{
    class InfoParams
    {
        private string _baseurl;
        private string _databasename;
        //
        public InfoParams()
        {
            var localSettings = Windows.Storage.ApplicationData.Current.LocalSettings;
            {
                var obj = localSettings.Values["DataserverBaseUrl"];
                if (obj == null)
                {
                    obj = "http://localhost:5984/";
                    localSettings.Values["DataserverBaseUrl"] = obj;
                }
                _baseurl = obj.ToString();
            }
            {
                var obj = localSettings.Values["DataserverDatabaseName"];
                if (obj == null)
                {
                    obj = "test";
                    localSettings.Values["DataserverDatabaseName"] = obj;
                }
                _databasename = obj.ToString();
            }
        }
        //
        public String BaseUrl
        {
            get
            {
                return _baseurl;
            }
        }
        public String DatabaseName
        {
            get
            {
                return _databasename;
            }
        }
    }// class InfoParams
}// namespace StatDataApp.Data
