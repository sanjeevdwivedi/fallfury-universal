using Coding4Fun.FallFury.Models;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml.Linq;
using Windows.Foundation;
using Windows.Storage;
using Windows.Storage.Streams;

namespace Coding4Fun.FallFury
{
    public sealed class ScoreWriter
    {
        async Task<Object> WriteTopScore(string name, string level, string score, MedalType medal)
        {
            bool isSuccessful = false;

            try
            {
                StorageFile file = null;
                StorageFolder folder = ApplicationData.Current.LocalFolder;

                folder = await folder.CreateFolderAsync("Meta",CreationCollisionOption.OpenIfExists);

                file = await folder.CreateFileAsync("highscores.xml",CreationCollisionOption.OpenIfExists);

                IRandomAccessStream readStream = await file.OpenAsync(FileAccessMode.Read);
                Stream inStream = Task.Run(() => readStream.AsStreamForRead()).Result;

                XDocument document = null;

                try
                {
                    string data;

                    using (StreamReader reader = new StreamReader(inStream))
                    {
                        data = reader.ReadToEnd();
                    }

                    document = XDocument.Parse(data);
                }
                catch
                {
                    document = new XDocument();
                    document.Add(new XElement("scores"));
                }

                XElement element = new XElement("score", new XAttribute("name", name),
                    new XAttribute("level", level), new XAttribute("score", score), new XAttribute("medal", (int)medal));

                document.Root.Add(element);

                file = await folder.CreateFileAsync("highscores.xml", Windows.Storage.CreationCollisionOption.ReplaceExisting);

                await FileIO.WriteTextAsync(file, document.ToString());

                isSuccessful = true;
            }
            catch
            {
                isSuccessful = false;
            }

            return isSuccessful;
        }

        public IAsyncOperation<Object> WriteScoreAsync(string name, string level, string score, MedalType medal)
        {
            return (IAsyncOperation<Object>)AsyncInfo.Run((CancellationToken token) => WriteTopScore(name,level,score, medal));
        }
    }
}
