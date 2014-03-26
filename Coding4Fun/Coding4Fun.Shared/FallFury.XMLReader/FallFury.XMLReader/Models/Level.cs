using System.Collections.Generic;
using System.Xml.Serialization;

namespace Coding4Fun.FallFury.Models
{
    public sealed class Level
    {
        public Bear GameBear { get; set; }
        public Meta LevelMeta { get; set; }
        public Obstacle[] Obstacles { get; set; }
        public Monster[] Monsters { get; set; }
        public Button[] Buttons { get; set; }
        public Powerup[] Powerups { get; set; }
    }
}
