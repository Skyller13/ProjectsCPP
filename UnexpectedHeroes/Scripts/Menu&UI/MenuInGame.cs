using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;


[System.Serializable]
public enum MenuType
{
    Options,
    Inventory,
    Quests,
    Team,
    Glossary,
    MainMenu
}

public class MenuInGame : MonoBehaviour
{
    [SerializeField] MenuManagement OptionsCanvas;
    [SerializeField] MenuManagement InventoryCanvas;
    [SerializeField] MenuManagement QuestsCanvas;
    [SerializeField] MenuManagement TeamCanvas;
    [SerializeField] MenuManagement GlossaryCanvas;
    [SerializeField] MenuManagement MainMenuCanvas;

    private GameObject player;

    SoundManager soundManager;

    public InputManager inputManager;
    public InGameMenuMap IGMMap;

    public MenuIngameStateMachine ingameStateMachine;

    // Start is called before the first frame update
    void Start()
    {
        MenuManagement[] canvas = GetComponentsInChildren<MenuManagement>(true);

        soundManager = Toolbox.Instance.GetManager<SoundManager>();
        inputManager = Toolbox.Instance.GetManager<InputManager>();

        ingameStateMachine = FindObjectOfType<MenuIngameStateMachine>();

        foreach (var item in canvas)
        {
            switch (item.Type)
            {
                case MenuType.Options:
                    OptionsCanvas = item;
                    break;
                case MenuType.Inventory:

                    InventoryCanvas = item;
                    break;
                case MenuType.Quests:
                    QuestsCanvas = item;
                    break;
                case MenuType.Team:
                    TeamCanvas = item;
                    break;
                case MenuType.Glossary:
                    GlossaryCanvas = item;
                    break;
                case MenuType.MainMenu:
                    MainMenuCanvas = item;
                    break;
                default:
                    break;
            }
        }
    }

    void Update()
    {
        if (player == null)
        {
            player = Toolbox.Instance.GetManager<GameManager>().GetPlayerUnit().gameObject;
        }

        if (IGMMap == null)
        {
            IGMMap = inputManager.GetInputMap<InGameMenuMap>();
        }

        //if (Input.GetKeyDown(KeyCode.I)
        //    || Input.GetButtonDown("Y")
        //    && player.GetComponent<PlayerController>().PlayerAction == PlayerController.currentAction.isMenu)
        //{
        //    player.GetComponent<PlayerController>().PlayerAction = PlayerController.currentAction.isMenu;
        //    OpenInventoryMenu();
        //}

        if (player.GetComponent<PlayerController>().PlayerAction == PlayerController.currentAction.isMenu)
        {
            soundManager.PauseAllSounds();
            soundManager.PauseMusic();
            soundManager.PauseAmbiant();
        }
        else if (player.GetComponent<PlayerController>().PlayerAction == PlayerController.currentAction.isBase)
        {
            soundManager.ResumeAllSounds();
            soundManager.ResumeMusic();
            soundManager.ResumeAmbiant();
        }
    }

    #region AllMenu
    public void OpenMainMenu()
    {
        MainMenuCanvas.gameObject.SetActive(true);

        ingameStateMachine.selectedTypeState = MenuIngameStateMachine.stateMenuIngame.GLOBAL;
    }

    public void OpenOptionsMenu()
    {
        OptionsCanvas.gameObject.SetActive(true);

        ingameStateMachine.selectedTypeState = MenuIngameStateMachine.stateMenuIngame.OPTION;
    }

    public void OpenQuestsMenu()
    {
        QuestsCanvas.gameObject.SetActive(true);

        ingameStateMachine.selectedTypeState = MenuIngameStateMachine.stateMenuIngame.QUEST;
    }

    public void OpenTeamMenu()
    {
        TeamCanvas.gameObject.SetActive(true);

        ingameStateMachine.selectedTypeState = MenuIngameStateMachine.stateMenuIngame.TEAM;
    }

    public void OpenInventoryMenu()
    {
        InventoryCanvas.gameObject.SetActive(true);

        ingameStateMachine.selectedTypeState = MenuIngameStateMachine.stateMenuIngame.INVENTORY;
    }

    public void OpenGlossaryMenu()
    {
        GlossaryCanvas.gameObject.SetActive(true);

        ingameStateMachine.selectedTypeState = MenuIngameStateMachine.stateMenuIngame.GLOSSARY;
    }

    #endregion

    public void QuitGame()
    {
        Application.Quit();
    }

    public void LoadLobby()
    {
        SceneManager.LoadScene(1);
    }

    public void ExitMenu()
    {
        //MenuManagement tempMenu = FindObjectOfType<MenuManagement>();

        //tempMenu.ExitMenu();

        if (ingameStateMachine.selectedTypeState == MenuIngameStateMachine.stateMenuIngame.GLOBAL)
        {
            IGMMap.ExitInGameMenuMap();

            if (MainMenuCanvas.gameObject.activeInHierarchy == false)
            {
                player.GetComponent<PlayerController>().PlayerAction = PlayerController.currentAction.isBase;
            }
        }
        else
        {
            ingameStateMachine.selectedTypeState = MenuIngameStateMachine.stateMenuIngame.GLOBAL;
        }

        
    }

    public void OpenMenu()
    {
        OpenMainMenu();

        EventSystem es = GameObject.Find("EventSystem").GetComponent<EventSystem>();
        es.SetSelectedGameObject(null);
        es.SetSelectedGameObject(es.firstSelectedGameObject);

        player.GetComponent<PlayerController>().PlayerAction = PlayerController.currentAction.isMenu;
    }
}
