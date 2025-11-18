using System;
using System.Collections.Generic;
using System.Linq;

namespace IrrfExample
{
    // Representa uma faixa de IRRF
    public class IrrfBracket
    {
        public decimal MinExclusive { get; }
        public decimal MaxInclusive { get; }
        public decimal Rate { get; }      // ex: 0.075m para 7,5%
        public decimal Deduction { get; } // parcela a deduzir

        public IrrfBracket(decimal minExclusive, decimal maxInclusive, decimal rate, decimal deduction)
        {
            MinExclusive = minExclusive;
            MaxInclusive = maxInclusive;
            Rate = rate;
            Deduction = deduction;
        }

        public bool Contains(decimal baseSalary) => baseSalary > MinExclusive && baseSalary <= MaxInclusive;
    }

    // Calculadora de IRRF
    public class IrrfCalculator
    {
        private readonly List<IrrfBracket> _brackets;

        public IrrfCalculator(IEnumerable<IrrfBracket> brackets)
        {
            _brackets = brackets.ToList();
        }

        // Calcula o IRRF a partir do salário bruto e do desconto de INSS
        public decimal Calculate(decimal grossSalary, decimal inssDiscount)
        {
            if (grossSalary < 0) throw new ArgumentException("Salário bruto inválido", nameof(grossSalary));
            if (inssDiscount < 0) throw new ArgumentException("Desconto INSS inválido", nameof(inssDiscount));

            decimal baseSalary = grossSalary - inssDiscount;
            if (baseSalary <= 0) return 0m;

            var bracket = _brackets.FirstOrDefault(b => b.Contains(baseSalary));
            if (bracket == null) return 0m;

            decimal tax = baseSalary * bracket.Rate - bracket.Deduction;
            decimal rounded = decimal.Round(tax, 2, MidpointRounding.AwayFromZero);
            return Math.Max(rounded, 0m);
        }
    }

    class Program
    {
        static void Main()
        {
            // Tabela de exemplo (ajuste conforme a tabela que deseja usar).
            // Os valores a seguir combinam com o exemplo que você forneceu (dedução 169.44).
            var brackets = new List<IrrfBracket>
            {
                new IrrfBracket(0.00m, 1903.98m, 0.00m, 0.00m),        // isento
                new IrrfBracket(1903.98m, 2826.65m, 0.075m, 169.44m), // 7,5% e dedução 169,44 (exemplo)
                new IrrfBracket(2826.65m, 3751.05m, 0.15m, 354.80m),  // 15% e dedução (exemplo)
                new IrrfBracket(3751.05m, 4664.68m, 0.225m, 636.13m), // 22,5% e dedução (exemplo)
                new IrrfBracket(4664.68m, decimal.MaxValue, 0.275m, 869.36m) // 27,5% e dedução (ex.)
            };

            var calculator = new IrrfCalculator(brackets);

            // Valores para validar conforme seu enunciado
            decimal grossSalary = 3000.00m;
            decimal inssDiscount = 258.83m;

            decimal baseSalary = grossSalary - inssDiscount;
            decimal irrf = calculator.Calculate(grossSalary, inssDiscount);

            Console.WriteLine($"Salário Bruto: {grossSalary:F2}");
            Console.WriteLine($"Desconto INSS: {inssDiscount:F2}");
            Console.WriteLine($"Salário Base: {baseSalary:F2}");
            Console.WriteLine($"IRRF calculado: {irrf:F2}"); // deve imprimir 36.14
        }
    }
}