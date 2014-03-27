using System.Xml.Serialization;

namespace Coding4Fun.FallFury.Models
{
    public sealed class Meta
    {
        public int Score { get; set; }
        public int ButtonPrice { get; set; }
        public LevelType LevelType { get; set; }
    }
}
