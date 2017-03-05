using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Linq.Expressions;
using System.Reflection;
using System.ComponentModel;
using System.Runtime.CompilerServices;
//
namespace GestInfoApp.Common
{
    public class InfoObservable : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        //
        public InfoObservable()
        {
        }// InfoObservable
        protected void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            var pp = this.PropertyChanged;
            if (pp != null)
            {
                var p = this;
                pp(p, new PropertyChangedEventArgs(propertyName));
            }
        }//NotifyPropertyChanged
        protected void NotifyPropertyChanged(Expression<Func<object>> expression)
        {
            var lambda = expression as LambdaExpression;
            MemberExpression memberExpression;
            if (lambda.Body is UnaryExpression)
            {
                var unaryExpression = lambda.Body as UnaryExpression;
                memberExpression = unaryExpression.Operand as MemberExpression;
            }
            else
            {
                memberExpression = lambda.Body as MemberExpression;
            }
            if (memberExpression != null)
            {
                var propertyInfo = memberExpression.Member as PropertyInfo;
                if (propertyInfo != null)
                    NotifyPropertyChanged(propertyInfo.Name);
            }
        }
    }// class InfoObservable
}// namespace GestInfoApp.Common
