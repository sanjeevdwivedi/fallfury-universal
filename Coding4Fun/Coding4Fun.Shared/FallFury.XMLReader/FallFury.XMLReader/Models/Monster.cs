using System.Xml.Serialization;

namespace Coding4Fun.FallFury.Models
{
    public sealed class Monster
    {
        public float MaxHealth { get; set; }
        public int Bonus { get; set; }
        public float Damage { get; set; }
        public float CriticalDamage { get; set; }
        public int DefaultAmmo { get; set; }
        public float X { get; set; }
        public float Y { get; set; }
        public float VelocityX { get; set; }
        public float Lifetime { get; set; }
        public MonsterType Type { get; set; }
        public float Scale { get; set; }
    }
}
