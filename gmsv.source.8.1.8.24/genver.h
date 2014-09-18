char *genver="
__VERSION_H__
_NEW_SERVER_
_SERVER_NUMS
SERVER_VERSION 'SMO v8.0.4.7a For Linux Debug Marking'
SERVER_VERSION 'SMO v8.0.4.7a For Linux'
UNIQUE_KEYCODEITEM	100
UNIQUE_KEYCODEPET	'i'
_SA_VERSION_70				// (可開放) WON ADD 石器時代7.0 的版本控制
_DEFAULT_PKEY 'smosmosmo'	// (可開放)( 石器 6.0 )
_RUNNING_KEY  '20070328'	// (可開放)( 石器 6.0 )
_VERSION_20010911	        // (可開放) 9.11 編碼修改 
_TEST_PETCREATE             // (不可開) ANDY 測寵的GM指令
_FAMILY_MANORNUM_CHANGE	// CoolFish 用來修改裝園數量(4.0)
_FIX_10_FMPOINT			//（可開放）吉仔ADD 冰莊
FAMILY_MAXHOME			10	// (4.0) 家族據點
MANORNUM                10	// (4.0) 莊園數量
FMPOINTNUM				10   // (4.0) 有據點家族的最大數量
FAMILY_MAXHOME			9	// (4.0) 家族據點
MANORNUM                9	// (4.0) 莊園數量
FMPOINTNUM				9   // (4.0) 有據點家族的最大數量
_FMVER21		        // (可開放) 家族職掌功能  
_PERSONAL_FAME	    	// (可開放) Arminius 8.30 家族個人聲望  
_EVERYONE_RIDE	    	// (可開放) Robin 0924 騎寵平民化  
__ATTACK_MAGIC          // (可開放)	提供戰場中的攻擊性咒術
_ADD_ACTION             // (可開放) 一般NPC動作狀況的設定
_UNIQUE_P_I	        	// (可開放) CoolFish 2001/10/11 
_NEWSAVE                // (可開放) Robin
_STORECHAR              // (可開放) Robin
_DROPCHECK              // (可開放) Robin 防止重複丟道具
_DROPCHECK2             // (可開放) Robin 防止重複丟道具
_PET_TRANS           	// (可開放) 寵物轉生功能
_NEW_WARPMAN        	// (可開放) NPC多重WARP功能
_PET_TALK	            // (可開放) 寵物對話
_PET_LIMITLEVEL	        // (可開放) 特殊寵設計(瑪蕾)
_MAP_NOEXIT	            // (可開放) 特殊地圖不得原登&原登回指定點
_GMRELOAD           	// (可開放) CoolFish: 2001/11/12 
_GAMBLE_ROULETTE	    // (可開放) 彥豪 賭場輪盤	寶寶小豬  
_FIX_GAMBLENUM          // (可開放) 輪盤修正 : 下線扣積分 
_GAMBLE_BANK	        // (可開放) 彥豪  賭場銀行  
_DROPSTAKENEW	        // (可開放) CoolFish:  賭場下注  
_PETRACE	            // (可開放) CoolFish: 2001/12/3 寵物競速場  
_NEWEVENT               // (可開放) CoolFish: 2001/12/5 新的任務旗標  
_ITEM_FIRECRACKER       // (可開放) 戰鬥中使用鞭炮可以把年獸嚇走
_MANOR_PKRULE           // (可開放)
_MANOR_FIXFMPK          // (可開放) WON ADD 家族約戰的 GM 指令，測試用的
_PSKILL_FALLGROUND		// (可開放) 寵物技能  (落馬術)
_ENEMY_FALLGROUND		// (可開放) NPC 落馬
_BATTLE_NEWPOWER		// (可開放)
_Item_MoonAct	        // (可開放) 月神的守護
_Item_DeathAct          // (可開放) 惡魔的首飾
_Item_PetAct            // (可開放) 馴獸戒指
_Item_ReLifeAct         // (可開放) 需要itemset1.txt 才能開) 替身娃娃
_BATTLESTEAL_FIX        // (可開放) 修正偷竊
_CFREE_petskill         // (可開放) 需要petskill1.txt 才能開) 新學習寵物技能技能
_data_newreadfile       // (可開放) 新的server setup.cf 檔案 包含 itemset1.txt petskill1.txt 兩個新檔
_ITEM_MAXUSERNUM        // (可開放) 新 itemset1.txt ITEM 使用次數
_PET_TALKPRO      		// (可開放) 寵物對話加強	& 改 END
_PRO_NPCFIX	     		// (可開放) NPC功能加強
_PRO_BATTLEENEMYSKILL	// (可開放) 戰鬥加強	提升NPC的AI技能
_NEW_WARPPOINT		    // (可開放) 傳送點多重條件判斷WARP
_FIX_CHECKITEM	      	// (可開放) 道具不做非法檢查
_ADD_ENCOUNT             // (可開放) WON 增加敵遭遇觸發修件 
_WON_PET_MAIL_LOG		 // (可開放) WON 玩家寄寵郵 LOG 
_FIX_FAMILY_PK_LIMIT	    // (可開放) WON 將可挑戰莊園的由前十大改為前二十大
_ADD_POOL_ITEM				// (可開放) WON 增加可寄放的道具
_ITEM_METAMO                // (可開放) shan metamo sprite. code:robin->shan
_ENEMY_ATTACK_AI            // (可開放) shan enemy attack mode. code:shan  
_ITEM_TIME_LIMIT            // (可開放) shan time limit of item. code:shan
_MIND_ICON                  // (可開放) shan show icon from char mind. code:shan
_BATTLENPC_WARP_PLAYER      // (可開放) shan npc warp player when n round. code:shan
_ITEM_WARP_FIX_BI           // (可開放) shan fix bbi to bi. code:shan
_SKILL_DAMAGETOHP			// (可開放) ANDY 嗜血技 
_Skill_MPDAMAGE				// (可開放) ANDY MP傷害
_WAEI_KICK					// (可開放) ANDY 跨星球踢人
_BATTLE_TIMESPEED			// (可開放) ANDY 戰鬥時間 測試(北京不必開，擋加速功能)
_NEW_RIDEPETS				// (可開放) ANDY 新騎寵
_FIXBUG_ATTACKBOW			// (可開放) ANDY 修正騎寵使用投擲武器bug
_NPCCHANGE_PLAYERIMG		// (可開放) ANDY NPC改變玩家圖號, 強制落馬,認玩家圖號 (測完)
_ITEM_ATTSKILLMAGIC			// (可開放) ANDY 道具技能  
_EQUIT_DEFMAGIC				// (可開放) ANDY 抗魔裝備
_FIX_MAGICDAMAGE			// (可開放) ANDY 修正魔法公式 三期開放
_ITEM_CONSTITUTION			// (可開放) ANDY 修正個人體質
_MAGIC_REHPAI				// (可開放) ANDY 補血AI
_TRANSER_MAN				// (可開放) ANDY 傳送師
_TAKE_ITEMDAMAGE			// (可開放) ANDY 道具損壞	ITEMSET2
_FIX_FIRECRACKER			// (可開放) ANDY 修正 鞭炮
_FIX_ITEMPROB				// (可開放) ANDY 修正 掉寶率
_ADD_DEAMGEDEFC				// (可開放) ANDY 額外傷害 & 附加攻擊	ITEMSET2
_FIX_MAXCHARMP				// (可開放) ANDY MP上限
_ITEM_INSLAY				// (可開放) ANDY 鑲寶石 動作
_ITEMSET2_ITEM				// (可開放) ANDY ITEMSET2
_ITEM_NPCCHANGE				// (可開放) ANDY 工匠 (提煉, 精工)
_PETSKILL_FIXITEM			// (可開放) ANDY 修復 && 同道具損壞
_FIX_SETWORKINT				// (可開放) ANDY (FIXWORKINT)
_ITEM_ADDEXP				// (可開放) vincent  道具:指定時間內增加經驗值上升幅度
_ITEM_ADDEXP2				// 智果時間可以累加 Robin
_FIX_METAMORIDE				// (可開放) ANDY
_CHECK_ITEMDAMAGE			// (可開放) 道具損壞修正	7/26
_SKILL_WILDVIOLENT_ATT		// (可開放) vincent  寵技:狂暴攻擊
_MAGIC_WEAKEN				// (可開放) vincent  精靈:虛弱
_SKILL_WEAKEN				// (可開放) vincent  寵技:虛弱//需要開#define _MAGIC_WEAKEN
_SKILL_SPEEDY_ATT			// (可開放) vincent  寵技:疾速攻擊
_SKILL_GUARDBREAK2			// (可開放) vincent  寵技:破除防禦2
_SKILL_SACRIFICE			// (可開放) vincent  寵技:救援
_PSKILL_MODIFY				// (可開放) ANDY 屬性強化攻擊
_PSKILL_MDFYATTACK			// (可開放) ANDY 攻擊轉化屬性
_MAGIC_DEFMAGICATT			// (可開放) ANDY 魔法防禦
_MAGIC_SUPERWALL			// (可開放) ANDY 鐵壁防禦
_OTHER_MAGICSTAUTS			// (可開放) ANDY 
_SKILL_TOOTH				// (可開放) ANDY   齒術
_MAGIC_DEEPPOISON			// (可開放) vincent  精靈:劇毒
_MAGIC_BARRIER				// (可開放) vincent  精靈:魔障
_MAGIC_NOCAST				// (可開放) vincent  精靈:沉默
_ITEM_CRACKER               // (可開放) vincent  道具:拉炮//需要開#define _MIND_ICON
_SKILL_DEEPPOISON			// (可開放) vincent  寵技:劇毒//需要開#define _MAGIC_DEEPPOISON
_SKILL_BARRIER				// (可開放) vincent  寵技:魔障//需要開#define _MAGIC_BARRIER
_SKILL_NOCAST				// (可開放) vincent  寵技:沉默//需要開#define _MAGIC_NOCAST
_SKILL_ROAR					// (可開放) vincent  寵技:大吼(克年獸)
_SKILL_REFRESH			    // (可開放) vincent  寵技:解除異常狀態
_ITEM_REFRESH               // (可開放) vincent  解除異常狀態道具
_MAGIC_TOCALL				// (可開放) kjl		 召喚	02/06/20 kjl
_ITEMSET3_ITEM				// (可開放) ANDY itemset3.txt
_SUIT_ITEM					// (可開放) ANDY (套裝) 來吉卡 需用itemset3.txt
_PETSKILL_SETDUCK			// (可開放) ANDY 蒙奇技能
_VARY_WOLF                  // (可開放) pet skill : vary wolf. code:shan
_USEWARP_FORNUM				// (可開放) ANDY 記憶羽毛 來吉卡
_IMPRECATE_ITEM				// (可開放) ANDY 特殊道具 來吉卡
_MAGICPET_SKILL				// (可開放) ANDY 魔法寵技能
_ITEM_CHECKWARES			// (可開放) ANDY 鎖定傳送不能
_NPC_REPLACEMENT			// (可開放) ANDY 置換戰鬥npc
_EMENY_CHANCEMAN			// (可開放) ANDY EMENY 選擇
_TYPE_TOXICATION			// (可開放) ANDY 中毒無法回血
_PET_FUSION					// (可開放) ANDY 寵物融合 enemybase1.txt
_NPC_FUSION					// (可開放) ANDY NPC寵物融合
_ITEM_EDITBASES				// (可開放) ANDY 營養劑
_PET_EVOLUTION				// (可開放) ANDY 進化表
_AVID_TRADETRYBUG			// (可開放) ANDY 防止交易中丟寵/道具 寄(寵物)郵件 石幣
_ASSESS_SYSEFFICACY			// (可開放) ANDY 評估系統效能 12/04 更新
_ASSESS_SYSEFFICACY_SUB		// (可開放) Robin 評估系統效能 增加檢查內迴圈
_PREVENT_TEAMATTACK			// (可開放) ANDY 防止同隊互打 12/04 更新
_ITEM_ORNAMENTS				// (可開放) ANDY 裝飾道具	12/04 更新 //出版部
_CHIKULA_STONE				// (可開放) ANDY 奇克拉之石 12/04 更新 //來吉卡 寵包
_SEND_EFFECT				// (可開放) WON  AC送下雪、下雨等特效 12/04 更新
_PETMAIL_DEFNUMS			// (可開放) ANDY 寵物郵件限制
_TEAM_KICKPARTY				// (可開放) ANDY 隊長踢人(非戰鬥狀態)
_PETSKILL_TIMID				// (可開放) ANDY 寵技-怯戰
_PETS_SELECTCON				// (可開放) ANDY 寵物不可點選出戰
_CHRISTMAS_REDSOCKS			// (可開放) ANDY 聖誕紅襪
_FIX_ARRAYBUG				// (可開放) ANDY 修正array 溢位
_USER_CHARLOOPS				// (可開放) ANDY 新用物件LOOP
_BATTLE_PROPERTY			// (可開放) ANDY 戰鬥屬性特性
_PETSKILL_PROPERTY			// (可開放) ANDY 無屬魔獸寵技
_ITEM_FIXALLBASE			// (可開放) ANDY 修復之石
_ITEM_LOVERPARTY			// (可開放) ANDY 情人節道具
_ITEM_FORUSERNAMES			// (可開放) ANDY 道具所有人
_BATTLECOMMAND_TIME			// (可開放) ANDY 防止玩家開緩速(回合戰鬥時間)
_NPCENEMY_ADDPOWER			// (可開放) ANDY 修改怪物的能力值
_PETSKILL_CANNEDFOOD		// (可開放) ANDY 寵物技能罐頭	for 6.0
_TEST_DROPITEMS				// (可開放) ANDY 測試丟寵
_MAP_WARPPOINT				// (可開放) ANDY Map WarpPoint
_DEFEND_TIMECHECK			// (可開放) ANDY 內部測試時間
_TREASURE_BOX				// (可開放) ANDY 寶箱
_BENEMY_EVENTRUN			// (可開放) ANDY 戰鬥npc給道具
_BATTLE_ABDUCTII			// (可開放) ANDY 旅途夥伴2
_BATTLE_LIGHTTAKE			// (可開放) ANDY 采光術
_BATTLE_ATTCRAZED			// (可開放) ANDY 瘋狂暴走
_STATUS_WATERWORD			// (可開放) ANDY 水世界狀態
_ITEM_WATERWORDSTATUS		// (可開放) ANDY 水世界狀態道具
_REEDIT_PETBBIS				// (可開放) ANDY 修正寵物波比圖形
_CAPTURE_FREES				// (可開放) ANDY 補抓條件
_THROWITEM_ITEMS			// (可開放) 丟擲後有獎品
_OBJSEND_C					// (可開放) ANDY _OBJSEND_C
_ADD_SHOW_ITEMDAMAGE		// (可開放) WON  顯示物品耐久度
_CHANGETRADERULE		    // (可開放) Syu ADD 交易規則修訂
_TRADE_SHOWHP				// (可開放) Syu ADD 交易顯示血量
_SHOOTCHESTNUT				// (可開放) Syu ADD 寵技：丟栗子
_SKILLLIMIT					// (可開放) Syu ADD 不得攻擊我方限制
_TRADESYSTEM2				// (可開放) Syu ADD 新交易系統
_NPC_NOSAVEPOINT			// (可開放) ANDY 紀錄點不存人物資料
_ACFMPK_LIST				// (可開放) ANDY 家族對戰列表修正
_PETSKILL2_TXT				// (可開放) ANDY petskill2.txt
_PETSKILL_CHECKTYPE			// (可開放) ANDY 寵物技能使用時機
_PETSKILL_TEAR				// (可開放) ANDY 寵技 撕裂傷口
_NPC_MAKEPAIR				// (可開放) ANDY NPC 配對
_ITEMSET4_TXT				// (可開放) ANDY itemset4.txt
_ITEM_PILENUMS				// (可開放) ANDY 道具素材堆疊 需 itemset4
_ITEM_PILEFORTRADE			// (可開放) ANDY 交易堆疊
_ITEM_EQUITSPACE			// (可開放) ANDY 新裝備欄位
_PET_LOSTPET				// (可開放) ANDY 遺失寵物搜尋計劃寵物
_ITEMSET5_TXT				// (可開放) ANDY itemset5.txt
_ITEMSET6_TXT				// (可開放) ANDY itemset6.txt
_EQUIT_ARRANGE				// (可開放) ANDY 擋格 需要 itemset5.txt
_EQUIT_SEQUENCE				// (可開放) ANDY 攻擊順序 需要 itemset5.txt
_EQUIT_ADDPILE				// (可開放) ANDY 增加可堆疊數 需要 itemset5.txt
_EQUIT_HITRIGHT				// (可開放) ANDY 命中 需要 itemset5.txt
_EQUIT_NEGLECTGUARD			// (可開放) ANDY 忽視目標防禦力% 需要 itemset6.txt
_EQUIT_NEWGLOVE				// (可開放) ANDY 手套欄位
_FM_JOINLIMIT				// (可開放) ANDY 加入家族條件限制
_FIX_MAX_GOLD				// (可開放) WON ADD 增加人物金錢上限 
_CK_ONLINE_PLAYER_COUNT		// (可開放) WON 計算線上人數 (要配合ac，否則會重開)	
_PET_SKILL_SARS				// (可開放) WON ADD 毒煞蔓延
_SONIC_ATTACK				// (可開放) WON ADD 音波攻擊
_NET_REDUCESEND				// (可開放) ANDY 減少DB資料傳送
_FEV_ADD_NEW_ITEM			// (可開放) Change 增加復活守精
_ALLDOMAN					// (可開放) Syu ADD 排行榜NPC
_LOCKHELP_OK				// (可開放) Syu ADD 鎖定不可加入戰鬥
_TELLCHANNEL				// (可開放) Syu ADD 密語頻道
_NEWREQUESTPROTOCOL			// (可開放) Syu ADD 新增Protocol要求細項
_OUTOFBATTLESKILL			// (可開放) Syu ADD 非戰鬥時技能Protocol
_CHAR_PROFESSION			// (可開放) WON ADD 人物職業欄位
_PROFESSION_SKILL			// (可開放) WON ADD 人物職業技能
_NPC_WELFARE				// (可開放) WON ADD 職業NPC
_PROFESSION_FIX_LEVEL		// (可開放) WON ADD 修正職業經驗值(北京&韓國不必開)
_GM_SIGUSR2					// (可開放) Change 下達sigusr2後執行 gm_sigusr_command檔案內的GM指令,若沒有指令就關閉GMSV
_GM_BROADCAST				// (可開放) WON ADD 客服公告系統
_NPC_ADDLEVELUP				// (可開放) ANDY NPC增加玩家等級
_CHAR_FIXDATADEF			// (可開放) ANDY 修正人物資料初始
_SIMPLIFY_ITEMSTRING		// (可開放) ANDY 簡化道具字串
_CHAR_NEWLOGOUT				// (可開放) ANDY 登出新機制
_CHATROOMPROTOCOL			// (可開放) Syu ADD 聊天室頻道 8/27
_CHAR_POOLITEM				// (可開放) ANDY 人物道具倉庫共用
_NPC_DEPOTITEM				// (可開放) ANDY 人物道具倉庫共用
_SIMPLIFY_PETSTRING			// (可開放) ANDY 簡化寵物存檔字串
_SIMPLIFY_ITEMSTRING2		// (可開放) ANDY 簡化道具字串2
_WOLF_TAKE_AXE				// (可開放) WON ADD 抓雙頭狼的限制
_FIX_UNNECESSARY			// (可開放) ANDY 修正不必要的判斷
_ITEM_MAGICRECOVERY			// (可開放) ANDY 新光鏡守
_PETSKILL_GYRATE			// (可開放) Change 寵技:迴旋攻擊
_PETSKILL_ACUPUNCTURE		// (可開放) Change 寵技:針刺外皮 (client端的_PETSKILL_ACUPUNCTURE也要開)
_PETSKILL_RETRACE			// (可開放) Change 寵技:追跡攻擊
_PETSKILL_HECTOR			// (可開放) Change 寵技:威嚇
_PETSKILL_FIREKILL			// (可開放) Change 寵技:火線獵殺
_PETSKILL_DAMAGETOHP		// (可開放) Change 寵技:暗月狂狼(嗜血技的變體) 
_PETSKILL_BECOMEFOX         // (可開放) Change 寵技:媚惑術
_FIX_MESSAGE				// (可開放) WON ADD 修改WORKER封包內容 (北京&韓國不必開)     
_PETSKILL_SHOWMERCY         // (可開) Change 寵技:手下留情
_NPC_ActionFreeCmp          // (可開) Change 替NPC參數加入新的比較方式
_NPC_ProfessionTrans      // (可開) Change 替職業NPC加入判斷轉生
_NPC_NewDelPet            // (可開) Change 替NPC加入刪除寵物(原本就有了,只是用warpman使用時不成功,所以先做一個頂一下)
_CHANGEGALAXY	// (可開) 星系移民　Robin 2003/10
_ALLDOMAN_DEBUG           // (可開) Change 修正重啟server後排行榜消失bug
_CHRISTMAS_REDSOCKS_NEW	  // (可開) Change 新聖誕紅襪
_PETSKILL_COMBINED      // (可開) Change 寵技:難得糊塗 [綜合魔法(依參數亂數取出魔法)]
_NPC_ADDWARPMAN1			// (可開放) Change npcgen_warpman增加功能(設定組隊人數、設定組隊男女人數)
_ITEM_QUITPARTY			// (可開放) Change 隊伍解散時特定道具即會消失
_ITEM_ADDEQUIPEXP       // (可開) Change 裝備後經驗值增加
_ITEM_LVUPUP            // (可開) Change 突破寵物等級限制的藥(玄武專用)
_PETSKILL_BECOMEPIG     // (可開) Change 寵技:烏力化
_ITEM_UNBECOMEPIG       // (可開) Change 烏力化的解藥
_PET_TALKBBI            // (可開) Change 增加判斷bbi
_ITEM_USEMAGIC			// (可開) Change 由道具使用魔法
_ITEM_PROPERTY          // (可開) Change 改變四屬性道具
_SUIT_ADDENDUM          // (可開) Change 替套裝增加功能 (_SUIT_ITEM要開)
_ITEM_CHECKDROPATLOGOUT   // (可開) Change 檢查回紀錄點時身上有無登出後消失道具,若有,不可回紀錄點
_FM_MODIFY				// 家族功能修改(開放)
_CHANNEL_MODIFY		// 頻道功能整合(開放)
_STANDBYPET		// Robin 待機寵封包
_PROSKILL_OPTIMUM	// Robin 職業技能處理最佳化
_PETSKILL_OPTIMUM	// Robin 寵物技能處理最佳化
_MAGIC_OPTIMUM		// Robin Magic table 最佳化
_AC_PIORITY		// Nuke 提高AC的優先權(可開)
_CHAR_POOLPET				// (可開放) Robin 人物寵物倉庫共用
_NPC_DEPOTPET				// (可開放) Robin 人物寵物倉庫共用
_TIMEOUTKICK	// (可開) 時段制踢人　Robin 2003/10
_MAGIC_RESIST_EQUIT			// (可開) WON ADD 職業抗性裝備
_LOG_OTHER // 雜項訊息專用LOG
_NPC_EXCHANGEMANTRANS   // (可開) Change 替exchangeman加入判斷轉生
_ANGEL_SUMMON	// Robin 天使召喚
_STREET_VENDOR		// 擺攤功能(開放)
_WATCH_EVENT			// 查詢任務功能(開放)
_NEW_MANOR_LAW		// 新莊園規則(開放)
_MAP_TIME					// 特殊地圖,會倒數(開放)
_PETSKILL_LER			// 雷爾技能(開放)
_FIX_MAGIC_RESIST				// (可開) Change 職業魔法抗性
_ADD_DUNGEON					// (可開) Change 追加地牢
_PROFESSION_ADDSKILL			// (可開) Change 追加職業技能
_ITEM_ADDPETEXP				// (可開) Change 增加寵物經驗值道具(可吃此道具寵物,平常不可獲得經驗值)
_TRANS_6						// (可開) Change 人物6轉
_PET_2TRANS					// (可開) Change 寵物2轉
_PET_2LIMITLEVEL				// (可開) Change 幫助寵物2轉的瑪蕾
_MAGICSTAUTS_RESIST			// (可開) Change (火電冰)抗性精靈
_EQUIT_RESIST					// (可開) Change 單一裝備抗性(可抗單一功能異常狀態)有抗火 雷 冰 虛弱 魔障 沉默 落馬
_SUIT_TWFWENDUM				// (可開) Change 四屬性套裝功能 (_SUIT_ITEM要開)
_EXCHANGEMAN_REQUEST_DELPET	// (可開) Change 替exchangeman的TYPE:REQUEST增加刪除寵物
_DEL_DROP_GOLD			// 刪除地上過時的石幣	Robin
_ITEM_STONE				// cyg 奇怪的石頭
_HELP_NEWHAND			// cyg 新創角色給予新手道具) 07.4.5吉仔修改 用於CF設置創建新人所送道具item1-item15
_DEF_FMFREETIME			// cyg GM指令 退出家族時間限制設為0
_DEF_GETYOU				// cyg GM指令 查詢自己兩格  圍內的玩家帳號
_DEF_NEWSEND			// cyg GM指令 傳送玩家時可加句子也可不加
_DEF_SUPERSEND			// cyg GM指令 可傳送已某帳號為中心附近3格內所有玩家
_SUIT_ADDPART3			// (可開) Change 套裝功能第三彈 迴避提高% 沉默抗性提高% 敏提高% 防提高% 攻提高%
_PETSKILL_2TIMID		// (可開) Change 怯戰第二版(狂獅怒吼)
CAVALRY_DEBUG			// (暫時開) Change 座騎攻擊除錯中
_SHOW_FUSION			// (可開) Change 融合寵物在CLIENT端只顯示轉生數,修改為融合寵顯示為融合(client端也要開)
_CONTRACT				// (可開) Robin 契約功能 
_FONT_SIZE				// (可開) Robin 字型大小功能
_PROSK99				// (可開) Change 檢查職技等級送白狼
_TIME_TICKET			// Robin 限時入場 森林捉寵用
_HALLOWEEN_EFFECT		// 萬聖節特效
_ADD_STATUS_2			// 新增傳送人物狀態第二類
_ADD_reITEM				// (可開) Change 檢查道具欄位空格
_ADD_NOITEM_BATTLE		// 新增NPC檢查人物身上沒道具會進入戰鬥
_PETSKILL_BATTLE_MODEL	// 寵物技能戰鬥模組
_RACEMAN				// cyg 獵寵大會新npc
_PETSKILL_ANTINTER		// (可開) Change 寵技:蟻之葬 參考_PETSKILL_2TIMID
_PETSKILL_REGRET		// (可開) Change 寵技:憾甲一擊 參考_SONIC_ATTACK
_SUIT_ADDPART4			// (可開) Change 套裝功能第四彈 中毒攻擊% 加強法師魔法(機率30%) 抵抗法師的魔法
_STONEAGEA8XXX_S						//吉仔 8.0開關
_ITEM_S_TEMP						//吉仔 8.0道具封包支持，此功能為臨時功能
_JZ_NEW_CONF						//吉仔 ADD 新CF字段 (不包括創建人物送道具字段)
_JZ_NEW_GMCOMMAND				//吉仔 ADD 新GM指令
_JZ_IMPROVE_GMCOMMAND		//吉仔 ADD 改進GM指令
_JZ_NEW_ITEM1						//吉仔 ADD 新增道具
_JZ_NEW_ADDMEMBERPOINT	//吉仔 ADD 會員點卷
_JZ_MANORTES						//莊園測試
<<Generated at 五 4月 20 04:18:14 CST 2007>>
";
