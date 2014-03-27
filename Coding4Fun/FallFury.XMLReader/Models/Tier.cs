using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Coding4Fun.FallFury.Models
{
    public sealed class Tier
    {
        public string Name { get; set; }
        public string[] LevelNames { get; set; }
        public string[] LevelFiles { get; set; }
    }
}
