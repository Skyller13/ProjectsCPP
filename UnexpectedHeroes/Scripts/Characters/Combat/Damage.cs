using System.Collections;
using System.Collections.Generic;
using UnityEngine;



public class Damage
{
	

	public SpellData.Category typeDMG = SpellData.Category.Physical;

	public SpellData.SpellElement type1 = SpellData.SpellElement.None;
	public SpellData.SpellElement type2 = SpellData.SpellElement.None;

	public float value = 1;

	#region Constructors
	public Damage()
	{

	}

	public Damage(float _value, SpellData.Category _type, SpellData.SpellElement _Elem1, SpellData.SpellElement _Elem2)
	{
		value = _value;
		typeDMG = _type;
		type1 = _Elem1;
		type2 = _Elem2;
	}
	#endregion

	public static float[,] tableElement =
	{
		//              terre   eau    air    feu    lumiere    tenebre
		/*terre*/	  { 0.5f,   1.5f,  1,     0.5f,  0.5f,      1.5f},
		/*eau*/		  { 0.5f,   0.5f,  1.5f,  1,     0.5f,      1.5f},
		/*air*/		  { 1,      0.5f,  0.5f,  1.5f,  1.5f,      0.5f},
		/*feu*/		  { 1.5f,   1,     0.5f,  0.5f,  1.5f,      0.5f},
		/*lumiere*/   { 1.5f,   1.5f,  0.5f,  0.5f,  0.5f,      1.5f},
		/*tenebre*/   { 0.5f,   0.5f,  1.5f,  1.5f,  1.5f,      0.5f}
	};

	public static float ReturnDamage(Damage damage, SpellData.SpellElement typeUnit1, SpellData.SpellElement typeUnit2)
	{
		float res = 0, denominator = 0;
		if (damage.type1 != SpellData.SpellElement.None && typeUnit1 != SpellData.SpellElement.None)
		{
			res += tableElement[(int)typeUnit1, (int)damage.type1];
			denominator++;
		}

		if (damage.type1 != SpellData.SpellElement.None && typeUnit2 != SpellData.SpellElement.None)
		{
			res += tableElement[(int)typeUnit2, (int)damage.type1];
			denominator++;
		}

		if (damage.type2 != SpellData.SpellElement.None && typeUnit1 != SpellData.SpellElement.None)
		{
			res += tableElement[(int)typeUnit1, (int)damage.type2];
			denominator++;
		}

		if (damage.type2 != SpellData.SpellElement.None && typeUnit2 != SpellData.SpellElement.None)
		{
			res += tableElement[(int)typeUnit2, (int)damage.type2];
			denominator++;
		}

		if (denominator > 0)
		{
			res /= denominator;
		}
		else
		{
			res = 1f;
		}
		

		return Mathf.Round(damage.value * res);
	}
}
