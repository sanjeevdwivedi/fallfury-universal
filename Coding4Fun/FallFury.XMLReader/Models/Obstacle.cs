using System.Xml.Serialization;

namespace Coding4Fun.FallFury.Models
{
    public sealed class Obstacle
    {
        public float X { get; set; }
        public float Y { get; set; }
        public bool InflictsDamage { get; set; }
        public float HealthDamage { get; set; }
        public float Rotation { get; set; }
        public float Scale { get; set; }
        public ObstacleType Type { get; set; }
    }
}
