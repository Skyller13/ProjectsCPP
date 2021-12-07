using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class MenuManager : MonoBehaviour
{
    public GameObject loadingScreen;
    public Slider slider;
    private float totalSceneProgress;
    public Text textTips;
    public CanvasGroup alpha;

    string[] tips;

    List<AsyncOperation> scenesLoading = new List<AsyncOperation>();

    public void LoadGame()
    {
        tips = new string[5];

        //define tips

        tips[0] = "Use left trigger to move in diagonal";
        tips[1] = "Use right joystick to change camera view";
        tips[2] = "Use right trigger and A, B, X or Y buttons to cast a spell";
        tips[3] = "Use directional pad to change direction without skipping turn";
        tips[4] = "Unexpected Heroes waiting for you !";

        //

        StartCoroutine(GenerateTips());

        loadingScreen.gameObject.SetActive(true);

        scenesLoading.Add(SceneManager.UnloadSceneAsync("Title Screen"));

        scenesLoading.Add(SceneManager.LoadSceneAsync("TestPathfinding3", LoadSceneMode.Single));

        StartCoroutine(GetSceneLoadProgress());
    }

    public IEnumerator GetSceneLoadProgress()
    {
        totalSceneProgress = 0;

        yield return new WaitForSeconds(1f);

        totalSceneProgress += scenesLoading[1].progress;

        totalSceneProgress = (totalSceneProgress / scenesLoading.Count) * 100f;

        slider.value = Mathf.RoundToInt(totalSceneProgress);

        yield return scenesLoading[1].isDone;
    }

    public int tipCount;
    public IEnumerator GenerateTips()
    {
        tipCount = Random.Range(0, tips.Length);
        textTips.text = tips[tipCount];

        while (loadingScreen.activeInHierarchy)
        {
            yield return new WaitForSeconds(2f);

            //change with alpha 

            tipCount++;

            if (tipCount > tips.Length)
            {
                tipCount = 0;
            }

            textTips.text = tips[tipCount];
        }
    }
}
