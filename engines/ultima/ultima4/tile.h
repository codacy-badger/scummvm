/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef ULTIMA4_TILE_H
#define ULTIMA4_TILE_H

#include "ultima/ultima4/direction.h"
#include "ultima/ultima4/types.h"
#include "ultima/ultima4/tileset.h"
#include "ultima/shared/std/containers.h"

namespace Ultima {
namespace Ultima4 {

class ConfigElement;
class Image;
class Tileset;
class TileAnim;

/* attr masks */
#define MASK_SHIP               	0x0001
#define MASK_HORSE              	0x0002
#define MASK_BALLOON            	0x0004
#define MASK_DISPEL             	0x0008
#define MASK_TALKOVER           	0x0010
#define MASK_DOOR               	0x0020
#define MASK_LOCKEDDOOR         	0x0040
#define MASK_CHEST              	0x0080
#define MASK_ATTACKOVER         	0x0100
#define MASK_CANLANDBALLOON     	0x0200
#define MASK_REPLACEMENT        	0x0400
#define MASK_WATER_REPLACEMENT		0x0800
#define MASK_FOREGROUND         	0x1000
#define MASK_LIVING_THING			0x2000


/* movement masks */
#define MASK_SWIMABLE           0x0001
#define MASK_SAILABLE           0x0002
#define MASK_UNFLYABLE          0x0004
#define MASK_CREATURE_UNWALKABLE 0x0008

/**
 * A Tile object represents a specific tile type.  Every tile is a
 * member of a Tileset.  
 */
class Tile : private Uncopyable {
public:
    Tile(Tileset *tileset);

    void loadProperties(const ConfigElement &conf);

    TileId getId() const                {return id;}
    const Common::String &getName() const       {return name;}
    int getWidth() const                {return w;}
    int getHeight() const               {return h;}
    int getFrames() const               {return frames;}
    int getScale() const                {return scale;}
    TileAnim *getAnim() const           {return anim;}
    Image *getImage();
    const Common::String &getLooksLike() const  {return looks_like;}

    bool isTiledInDungeon() const       {return tiledInDungeon;}
    bool isLandForeground() const       {return foreground;}
    bool isWaterForeground() const      {return waterForeground;}

    int canWalkOn(Direction d) const   {return DIR_IN_MASK(d, rule->walkonDirs);}
	int canWalkOff(Direction d) const  { return DIR_IN_MASK(d, rule->walkoffDirs); }

    /**
     * All tiles that you can walk, swim, or sail on, can be attacked over. All others must declare
     * themselves
     */
	int  canAttackOver() const      {return isWalkable() || isSwimable() || isSailable() || (rule->mask & MASK_ATTACKOVER); }
	int  canLandBalloon() const     {return rule->mask & MASK_CANLANDBALLOON; }
	int  isLivingObject() const     {return rule->mask & MASK_LIVING_THING; }
	int  isReplacement() const      {return rule->mask & MASK_REPLACEMENT; }
	int  isWaterReplacement() const {return rule->mask & MASK_WATER_REPLACEMENT; }

	int  isWalkable() const         {return rule->walkonDirs > 0; }
    bool isCreatureWalkable() const {return canWalkOn(DIR_ADVANCE) && !(rule->movementMask & MASK_CREATURE_UNWALKABLE);}
    bool isDungeonWalkable() const;
    bool isDungeonFloor() const;
    int  isSwimable() const         {return rule->movementMask & MASK_SWIMABLE;}
    int  isSailable() const         {return rule->movementMask & MASK_SAILABLE;}
    bool isWater() const            {return (isSwimable() || isSailable());}
    int  isFlyable() const          {return !(rule->movementMask & MASK_UNFLYABLE);}
    int  isDoor() const             {return rule->mask & MASK_DOOR;}
    int  isLockedDoor() const       {return rule->mask & MASK_LOCKEDDOOR;}
    int  isChest() const            {return rule->mask & MASK_CHEST;}
    int  isShip() const             {return rule->mask & MASK_SHIP;}
    bool isPirateShip() const       {return name == "pirate_ship";}
    int  isHorse() const            {return rule->mask & MASK_HORSE;}
    int  isBalloon() const          {return rule->mask & MASK_BALLOON;}
    int  canDispel() const          {return rule->mask & MASK_DISPEL;}
    int  canTalkOver() const        {return rule->mask & MASK_TALKOVER;}
    TileSpeed getSpeed() const      {return rule->speed;}
    TileEffect getEffect() const    {return rule->effect;}

    bool isOpaque() const;
    bool isForeground() const;
    Direction directionForFrame(int frame) const;
    int frameForDirection(Direction d) const;

    static void resetNextId()                       {nextId = 0;}
    static bool canTalkOverTile(const Tile *tile)   {return tile->canTalkOver() != 0;}
    static bool canAttackOverTile(const Tile *tile) {return tile->canAttackOver() != 0;}
    void deleteImage();

private:
    void loadImage();

private:
    TileId id;          /**< an id that is unique across all tilesets */
    Common::String name;        /**< The name of this tile */
    Tileset *tileset;   /**< The tileset this tile belongs to */
    int w, h;           /**< width and height of the tile */
    int frames;         /**< The number of frames this tile has */
    int scale;          /**< The scale of the tile */
    TileAnim *anim;     /**< The tile animation for this tile */    
    bool opaque;        /**< Is this tile opaque? */

    bool foreground;    /**< As a maptile, is a foreground that will search neighbour maptiles for a land-based background replacement. ex: chests */
    bool waterForeground;/**< As a maptile, is a foreground that will search neighbour maptiles for a water-based background replacement. ex: chests */

    TileRule *rule;     /**< The rules that govern the behavior of this tile */
    Common::String imageName;   /**< The name of the image that belongs to this tile */
    Common::String looks_like;  /**< The name of the tile that this tile looks exactly like (if any) */    

    Image *image;       /**< The original image for this tile (with all of its frames) */
    bool tiledInDungeon;
    Std::vector<Direction> directions;

    Common::String animationRule;


    static TileId nextId;
};

} // End of namespace Ultima4
} // End of namespace Ultima

#endif