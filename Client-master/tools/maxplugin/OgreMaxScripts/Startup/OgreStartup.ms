
utility AnimManagerUtil "Ogre Animation Manager"  -- define the utility name and description string 
( 
	global OgreActorManagerUtility
	label OgreActorManagerUtilVersion "Version: 1.0.0" align:#center

    include "OgreMaxScripts/AnimationManager.ms"

	on  AnimManagerUtil open do 
	( 
		addRollout OgreActorManagerRollout r_dialogue rolledUp:false
		addRollout OgreActorSequenceRollout r_dialogue rolledUp:false
		addRollout MaterialSequenceRollout r_dialogue rolledUp:true
		addRollout ActorTriggerRollout r_dialogue rolledUp:true	
		addRollout ActorLayerRollout r_dialogue rolledUp:true
		OgreActorManagerUtility = OgreActorManagerRollout
		callbacks.addScript #filePostOpen "try(OgreActorManagerUtility.RefreshAll());catch()" id:#OgreActorManagerUtilPostOpen
	) 

	on  AnimManagerUtil close do 
	( 
		callbacks.removeScripts #filePostOpen id:#OgreActorManagerUtilPostOpen
		OgreActorManagerUtility = undefined
		removeRollout OgreActorManagerRollout 
		removeRollout OgreActorSequenceRollout
		removeRollout MaterialSequenceRollout  
		removeRollout ActorTriggerRollout
		removeRollout ActorLayerRollout 
	) 
)