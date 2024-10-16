
-----------------------------------------------------------------------------
-- This utility was designed to assist with Bone Level-of-detail setup.
-----------------------------------------------------------------------------
macroscript AnimManagerUtil category:"Ogre Animation Tools" 
	buttonText:"Ogre Animation Manager" toolTip:"Ogre Animation Manager" Icon:#("OgreIcon", 4)
( 
	global OgreActorManagerUtility
    local r_dialogue = undefined
    include "OgreMaxScripts\AnimationManager.ms"

    function CleanUp =
    (
        -- close the oldrollout floater if it exists
	    try(callbacks.removeScripts #filePostOpen id:#OgreActorManagerUtilPostOpen);catch()
        try(callbacks.removeScripts #systemPreReset id:#OgreActorManagerUtilPreReset);catch()
	    try(OgreActorManagerUtility = undefined);catch()
	    try(closerolloutfloater r_dialogue);catch()
    )

	on execute do
    (
        CleanUp()
        
        -- put up new rollout floater and add rollout to it
        r_dialogue = newrolloutfloater "Animation Manager Utility" 220 700
		addRollout OgreActorManagerRollout r_dialogue rolledUp:false
		addRollout OgreActorSequenceRollout r_dialogue rolledUp:false
		addRollout MaterialSequenceRollout r_dialogue rolledUp:true
		addRollout ActorTriggerRollout r_dialogue rolledUp:true	
		addRollout OgreActorLayerRollout r_dialogue rolledUp:true	
		OgreActorManagerUtility = OgreActorManagerRollout
		callbacks.addScript #filePostOpen "try(OgreActorManagerUtility.RefreshAll());catch()" id:#OgreActorManagerUtilPostOpen
		callbacks.addScript #systemPreReset "CleanUp()" id:#OgreActorManagerUtilPreReset
		-- end of Macro Script; rollout takes over
    )
)
