namespace Coding4Fun.FallFury.Models
{
    public sealed class TopScoreEntry
    {
        public string Name { get; set; }
        public string Level { get; set; }
        public int Score { get; set; }
        public MedalType Medal { get; set; }
    }
}
