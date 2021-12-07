using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class HUD : MonoBehaviour
{
    UIManager UIM;
    GameManager GameManager;

    GameObject Player;

    [SerializeField] Button endTurn;
    [SerializeField] Text defineTurn;
    [SerializeField] Button attackList;

    [SerializeField] GameObject wheel;
    [SerializeField] GameObject TargetMenu;
    bool isWheelActive = false;

    //[SerializeField] GameObject attackListObject;
    bool isAttackList = false;

    #region TeamUI

    public struct PlayerUIData
    {
        public UIManager.JaugeUI HP;
        public UIManager.JaugeUI Mana;
        public UIManager.JaugeUI Food;
    }

    public struct AllyUIData
    {
        public UIManager.JaugeUI HP;
        public UIManager.JaugeUI Mana;
    }

    [System.Serializable]
    public struct PlayerUILink
    {
        public Image head;
        public Image health;
        public Image mana;

        public Image food;

        public Text textHP;
        public Text textMana;

        public Text textFood;
    }

    [System.Serializable]
    public struct AllyUILink
    {
        public Image head;
        public Image health;
        public Image mana;

        public Text textHP;
        public Text textMana;
    }

    [SerializeField] PlayerUIData playerUIData = new PlayerUIData();
    [SerializeField] AllyUIData ally1UIData = new AllyUIData();
    [SerializeField] AllyUIData ally2UIData = new AllyUIData();

    [SerializeField] PlayerUILink playerUILink = new PlayerUILink();
    [SerializeField] AllyUILink ally1UILink = new AllyUILink();
    [SerializeField] AllyUILink ally2UILink = new AllyUILink();

    #endregion

    // Start is called before the first frame update
    void Start()
    {
        UIM = Toolbox.Instance.GetManager<UIManager>();
        GameManager = Toolbox.Instance.GetManager<GameManager>();

        GameManager.ONManagersInitialized += Init;
    }

    private void OnDestroy()
    {
        GameManager.ONManagersInitialized -= Init;
    }

    void Init()
    {
        wheel.SetActive(false);
        playerUIData.HP = UIM.PlayerHP;
        playerUIData.Mana = UIM.PlayerMana;
        playerUIData.Food = UIM.PlayerFood;

        ally1UIData.HP = UIM.Ally1HP;
        ally1UIData.Mana = UIM.Ally1Mana;

        ally2UIData.HP = UIM.Ally2HP;
        ally2UIData.Mana = UIM.Ally2Mana;

        endTurn.onClick.AddListener(CallIncrementTurn);
    }


    // Update is called once per frame
    void Update()
    {
        if (UIM.IsInit)
        {
            if (Player == null)
            {
                Player = FindObjectOfType<BookController>().gameObject;
            }

            switch (GameManager.CurrentTypeOfTurn)
            {
                case Unit.TypeUnit.Player:
                    defineTurn.text = "Tour Joueur";
                    break;
                case Unit.TypeUnit.Ally:
                    defineTurn.text = "Tour Allié";
                    break;
                case Unit.TypeUnit.Enemy:
                    defineTurn.text = "Tour Ennemi";
                    break;
                default:
                    break;
            }

            if (GameManager.xbox)
            {
                if (Input.GetButton("RB"))
                {
                    isWheelActive = true;
                    wheel.SetActive(true);
                }
                else
                {
                    isWheelActive = false;
                    wheel.SetActive(false);
                }

            }
            else
            {
                if (Input.GetKey(KeyCode.W))
                {
                    isWheelActive = true;
                    wheel.SetActive(true);

                    //if (isWheelActive && isAttackList == false)
                    //{
                    //    attackListObject.SetActive(false);
                    //}
                    //else if (isWheelActive && isAttackList == true)
                    //{
                    //    attackListObject.SetActive(true);
                    //}
                }
                else
                {
                    isWheelActive = false;
                    wheel.SetActive(false);
                }
            }

            if (GameManager.GetPlayerUnit().gameObject.GetComponent<PlayerController>().PlayerAction
                == PlayerController.currentAction.isTargeting)
            {
                TargetMenu.SetActive(true);
            }
            else
            {
                TargetMenu.SetActive(false);
            }


            if (Input.GetKey(KeyCode.K))
            {
                if (Input.GetMouseButtonDown(0))
                {
                    UIM.CreateFloatingText("24", Player.transform.position);
                }
            }

            #region Player

            playerUILink.textHP.text = playerUIData.HP.ToString();
            playerUILink.textMana.text = playerUIData.Mana.ToString();
            //playerUILink.textFood.text = playerUIData.Food.current + "%";

            playerUILink.health.fillAmount = playerUIData.HP.GetPercentage();
            playerUILink.mana.fillAmount = playerUIData.Mana.GetPercentage();
            //playerUILink.food.fillAmount = playerUIData.Food.GetPercentage();

            #endregion

            //#region Ally1

            //ally1UILink.textHP.text = ally1UIData.HP.ToString();
            //ally1UILink.textMana.text = ally1UIData.Mana.ToString();

            //ally1UILink.health.fillAmount = ally1UIData.HP.GetPercentage();
            //ally1UILink.mana.fillAmount = ally1UIData.Mana.GetPercentage();

            //#endregion

            //#region Ally2

            //ally2UILink.textHP.text = ally2UIData.HP.ToString();
            //ally2UILink.textMana.text = ally2UIData.Mana.ToString();

            //ally1UILink.health.fillAmount = ally1UIData.HP.GetPercentage();
            //ally1UILink.mana.fillAmount = ally1UIData.Mana.GetPercentage();

            //#endregion
        }

    }

    void CallIncrementTurn()
    {
        GameManager.IncrementTurn();
    }

    public void ActiveWheel()
    {
        isWheelActive = !isWheelActive;
    }

    public void ActiveAttackList()
    {
        isAttackList = !isAttackList;
    }
}
