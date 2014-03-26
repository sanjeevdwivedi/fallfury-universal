using System.Xml.Serialization;

namespace Coding4Fun.FallFury.Models
{
    public sealed class Bear
    {
        public float MaxHealth { get; set; }
        public float Damage { get; set; }
        public float CriticalDamage { get; set; }
        public int DefaultAmmo { get; set; }
        public float StartPosition { get; set; }
        public float Velocity { get; set; }
    }
}
