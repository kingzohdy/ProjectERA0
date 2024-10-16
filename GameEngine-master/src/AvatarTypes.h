
#ifndef __AvatarTypes_H__
#define __AvatarTypes_H__

namespace Ogre
{
	#define MAX_AVATAR_SLOT 32
	#define AVATAR_BIND_CLASS 400
	#define AVATAR_VISUAL_CLASS 1000
/*
	enum
	{
		DB_Char = 0,
		DB_HairGeosets = 1,
		DB_FacialHairStyle = 2,
		DB_Items = 3,
		DB_ItemDisp = 4,
	};

	enum ItemTypes
	{
		IT_ALL = 0,
		IT_HEAD,
		IT_NECK,
		IT_SHOULDER,
		IT_SHIRT,
		IT_CHEST,
		IT_BELT,
		IT_PANTS,
		IT_BOOTS,
		IT_BRACERS,
		IT_GLOVES,
		IT_RINGS,
		IT_ACCESSORY,
		IT_DAGGER,
		IT_SHIELD,
		IT_BOW,
		IT_CAPE,
		IT_2HANDED,
		IT_QUIVER,
		IT_TABARD,
		IT_ROBE,
		IT_1HANDED,
		IT_CLAW,
		IT_OFFHAND,
		IT_UNUSED, // unused?
		IT_THROWN,
		IT_GUN,

		NUM_ITEM_TYPES
	};

	class ItemDB
	{
	public:
		static const size_t Id = 0;
		static const size_t Model = 1;
		static const size_t Itemclass = 2;
		static const size_t Subclass = 3;
		static const size_t Type = 4;
		static const size_t Sheath = 5;
		static const size_t Quality = 6;
		static const size_t Name = 7;
	};
*/
	class ItemDisplayDB
	{
	public:
		/// Fields
		enum
		{
			ItemDisplayID = 0,
			Model,
			Model2,
			Model3,
			Skin,
			Skin2,
			Icon,
			GeosetA,
			GeosetB,
			GeosetC,
			GeosetD,
			GeosetE,
			Flags,
			TexA,
			TexB,
			TexC,
			TexD,
			TexE,
			TexF,
			TexG,
			TexH,
			Visuals,

			NoType
		};
	};

}

#endif
