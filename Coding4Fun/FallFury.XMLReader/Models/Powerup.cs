using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Coding4Fun.FallFury.Models
{
    public sealed class Powerup
    {
        public PowerupCategory Category { get; set; }
        public PowerupType Type { get; set; }
        public float X { get; set; }
        public float Y { get; set; }
        public float Effect { get; set; }
        public float Lifespan { get; set; }
    }
}
