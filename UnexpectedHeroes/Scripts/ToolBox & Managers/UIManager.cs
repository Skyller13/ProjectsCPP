using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
	[System.Serializable]
	public class JaugeUI
	{
		public float current = 0;
		public float max = 0;
		public float percentage = 0;

		public JaugeUI()
		{

		}

		public JaugeUI(float _current, float _max)
		{
			current = _current;
			max = _max;
			percentage = current / max;
		}

		public float GetPercentage()
		{
			return percentage = current / max;
		}

		public override string ToString()
		{
			return current.ToString() + " / " + max.ToString();
		}
	}

	GameManager gameManager;
	Unit Player;

	public JaugeUI PlayerHP = new JaugeUI();
	public JaugeUI PlayerMana = new JaugeUI();
	public JaugeUI PlayerFood = new JaugeUI();

	public JaugeUI Ally1HP = new JaugeUI();
	public JaugeUI Ally1Mana = new JaugeUI();

	public JaugeUI Ally2HP = new JaugeUI();
	public JaugeUI Ally2Mana = new JaugeUI();

	private static GameObject popupText;
	private static GameObject canvas;

	public bool IsInit = false;

	
	public void KapyStart(GameManager _GM)
	{
		gameManager = _GM;

		_GM.ONManagersInitialized += Initialize;
	}

	private void OnDestroy()
	{
		gameManager.ONManagersInitialized -= Initialize;
	}

	void Initialize()
    {
		Player = gameManager.GetPlayerUnit();

		canvas = FindObjectOfType<HUD>().gameObject;

		IsInit = true;
	}

    // Update is called once per frame
    void Update()
    {
		if (IsInit)
		{
			if (Player)
			{
				Player.GetCurrentHP(out PlayerHP.current, out PlayerHP.max);
				Player.GetCurrentMana(out PlayerMana.current, out PlayerMana.max);
			}

			if (popupText == null)
			{
				popupText = Resources.Load<GameObject>("Prefabs/PopupText");
			}
		}
		
	}

	public void CreateFloatingText(string text, Vector3 location)
	{
		Vector2 screenPosition = Camera.main.WorldToScreenPoint(location + Random.insideUnitSphere * 0.5f, Camera.main.stereoActiveEye);
		GameObject instance = Instantiate(popupText);

		instance.transform.SetParent(canvas.transform);
		instance.transform.position = screenPosition;
		instance.GetComponent<FloatingText>().SetText(text);
	}
}
