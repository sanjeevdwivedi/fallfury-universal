using System;
using System.IO;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading;
using System.Threading.Tasks;
using System.Xml.Linq;
using Coding4Fun.FallFury.Models;
using Windows.Foundation;
using Windows.Storage;
using Windows.Storage.Streams;
using System.Linq;
using System.Diagnostics;

namespace Coding4Fun.FallFury
{
    public sealed class Reader
    {
        async Task<Object> ReadXml(string fileName, XmlType type)
        {
            StorageFile file = null;
            StorageFolder folder = null;

            if (type == XmlType.LEVEL || type == XmlType.TIERS)
            {
                folder = Windows.ApplicationModel.Package.Current.InstalledLocation;
                folder = await folder.GetFolderAsync("Levels");
            }
            else if (type == XmlType.HIGHSCORE)
            {
                folder = ApplicationData.Current.LocalFolder;
                folder = await folder.GetFolderAsync("Meta");
            }

            file = await folder.GetFileAsync(fileName);

            Object returnValue = null;
            string data;

            using (IRandomAccessStream readStream = await file.OpenAsync(FileAccessMode.Read))
            {
                using (Stream inStream = Task.Run(() => readStream.AsStreamForRead()).Result)
                {
                    using (StreamReader reader = new StreamReader(inStream))
                    {
                        data = reader.ReadToEnd();
                    }
                }
            }

            if (type == XmlType.LEVEL)
            {
                returnValue = ExtractLevelData(data);
            }
            else if (type == XmlType.HIGHSCORE)
            {
                returnValue = ExtractScoreData(data);
            }
            else if (type == XmlType.TIERS)
            {
                returnValue = ExtractTierData(data);
            }

            return returnValue;
        }

        public IAsyncOperation<Object> ReadXmlAsync(string filename, XmlType type)
        {
            return (IAsyncOperation<Object>)AsyncInfo.Run((CancellationToken token) => ReadXml(filename, type));
        }

        TierSet ExtractTierData(string tierString)
        {
            TierSet set = new TierSet();

            XDocument document = XDocument.Parse(tierString);

            set.Tiers = new Tier[document.Root.Elements().Count()];

            int tierCounter = 0;

            foreach (XElement element in document.Root.Elements())
            {
                Tier tier = new Tier();
                tier.Name = element.Attribute("name").Value;

                tier.LevelFiles = new string[element.Elements("level").Count()];
                tier.LevelNames = new string[element.Elements("level").Count()];

                
                int count = 0;
                foreach (XElement lElement in element.Elements("level"))
                {
                    tier.LevelFiles[count] = lElement.Attribute("file").Value;
                    tier.LevelNames[count] = lElement.Attribute("name").Value;
                    count++;
                }

                set.Tiers[tierCounter] = tier;
                tierCounter++;
            }

            return set;
        }

        TopScoreSet ExtractScoreData(string scoreString)
        {
            XDocument document = XDocument.Parse(scoreString);

            TopScoreSet set = new TopScoreSet();
            set.Scores = new TopScoreEntry[document.Root.Elements().Count()];

            int count = 0;

            foreach (XElement element in document.Root.Elements())
            {
                TopScoreEntry entry = new TopScoreEntry();
                entry.Name = element.Attribute("name").Value;
                entry.Level = element.Attribute("level").Value;
                entry.Score = Convert.ToInt32(element.Attribute("score").Value);
                entry.Medal = (MedalType)Convert.ToInt32(element.Attribute("medal").Value);

                Debug.WriteLine(entry.Medal);

                set.Scores[count] = entry;

                count++;
            }

            return set;
        }

        Level ExtractLevelData(string levelString)
        {
            XDocument document = XDocument.Parse(levelString);

            Level level = new Level();
            level.LevelMeta = new Meta();
         
            level.LevelMeta.Score = Convert.ToInt32(document.Root.Element("meta").Attribute("score").Value);
            level.LevelMeta.ButtonPrice = Convert.ToInt32(document.Root.Element("meta").Attribute("buttonPrice").Value);
            level.LevelMeta.LevelType = (LevelType)Convert.ToInt32(document.Root.Attribute("type").Value);

            int count = document.Root.Element("obstacles").Elements().Count();
            level.Obstacles = new Obstacle[count];

            count = 0;
            foreach (XElement element in document.Root.Element("obstacles").Elements())
            {
                Obstacle obstacle = new Obstacle();
                obstacle.HealthDamage = Convert.ToSingle(element.Attribute("healthDamage").Value);
                obstacle.InflictsDamage = Convert.ToBoolean(element.Attribute("inflictsDamage").Value);
                obstacle.Rotation = Convert.ToSingle(element.Attribute("rotation").Value);
                obstacle.Scale = Convert.ToSingle(element.Attribute("scale").Value);
                obstacle.X = Convert.ToSingle(element.Attribute("x").Value);
                obstacle.Y = Convert.ToSingle(element.Attribute("y").Value);
                obstacle.Type = (ObstacleType)Convert.ToInt32(element.Attribute("type").Value);
                level.Obstacles[count] = obstacle;
                count++;
            }

            count = document.Root.Element("monsters").Elements().Count();
            level.Monsters = new Monster[count];

            count = 0;
            foreach (XElement element in document.Root.Element("monsters").Elements())
            {
                Monster monster = new Monster();
                monster.CriticalDamage = Convert.ToSingle(element.Attribute("criticalDamage").Value);
                monster.Damage = Convert.ToSingle(element.Attribute("damage").Value);
                monster.DefaultAmmo = Convert.ToInt32(element.Attribute("defaultAmmo").Value);
                monster.MaxHealth = Convert.ToSingle(element.Attribute("maxHealth").Value);
                monster.X = Convert.ToSingle(element.Attribute("x").Value);
                monster.Y = Convert.ToSingle(element.Attribute("y").Value);
                monster.VelocityX = Convert.ToSingle(element.Attribute("velocityX").Value);
                monster.Lifetime = Convert.ToSingle(element.Attribute("lifetime").Value);
                monster.Type = (MonsterType)Convert.ToInt32(element.Attribute("type").Value);
                monster.Bonus = Convert.ToInt32(element.Attribute("bonus").Value);
                monster.Scale = Convert.ToSingle(element.Attribute("scale").Value);

                level.Monsters[count] = monster;
                count++;
            }

            count = document.Root.Element("buttons").Elements().Count();
            level.Buttons = new Button[count];

            count = 0;
            foreach (XElement element in document.Root.Element("buttons").Elements())
            {
                Button button = new Button();

                button.X = Convert.ToSingle(element.Attribute("x").Value);
                button.Y = Convert.ToSingle(element.Attribute("y").Value);

                level.Buttons[count] = button;
                count++;
            }

            count = document.Root.Element("powerups").Elements().Count();
            level.Powerups = new Powerup[count];

            count = 0;
            foreach (XElement element in document.Root.Element("powerups").Elements())
            {
                Powerup powerup = new Powerup();

                powerup.X = Convert.ToSingle(element.Attribute("x").Value);
                powerup.Y = Convert.ToSingle(element.Attribute("y").Value);
                powerup.Category = (PowerupCategory)Convert.ToInt32(element.Attribute("category").Value);
                powerup.Type = (PowerupType)Convert.ToInt32(element.Attribute("type").Value);
                powerup.Lifespan = Convert.ToSingle(element.Attribute("lifespan").Value);
                powerup.Effect = Convert.ToSingle(element.Attribute("effect").Value);

                level.Powerups[count] = powerup;
                count++;
            }

            Bear bear = new Bear();
            XElement bearElement = document.Root.Element("bear");
            bear.CriticalDamage = Convert.ToSingle(bearElement.Attribute("criticalDamage").Value);
            bear.Damage = Convert.ToSingle(bearElement.Attribute("damage").Value);
            bear.DefaultAmmo = Convert.ToInt32(bearElement.Attribute("defaultAmmo").Value);
            bear.MaxHealth = Convert.ToSingle(bearElement.Attribute("maxHealth").Value);
            bear.StartPosition = Convert.ToSingle(bearElement.Attribute("startPosition").Value);
            bear.Velocity = Convert.ToSingle(bearElement.Attribute("velocity").Value);

            level.GameBear = bear;
            
            return level;
        }
    }
}
