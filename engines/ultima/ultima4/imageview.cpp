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

#include "ultima/ultima4/debug.h"
#include "ultima/ultima4/error.h"
#include "ultima/ultima4/image.h"
#include "ultima/ultima4/imagemgr.h"
#include "ultima/ultima4/imageview.h"
#include "ultima/ultima4/settings.h"

namespace Ultima {
namespace Ultima4 {

ImageView::ImageView(int x, int y, int width, int height) : View(x, y, width, height) {
}

ImageView::~ImageView() {
}

/**
 * Draw the image at the optionally specified offset.
 */
void ImageView::draw(const Common::String &imageName, int x, int y) {
    ImageInfo *info = imageMgr->get(imageName);
    if (info) {
        info->image->draw(SCALED(this->x + x), SCALED(this->y + y));
        return;
    }

    SubImage *subimage = imageMgr->getSubImage(imageName);
    if (subimage) {
        info = imageMgr->get(subimage->srcImageName);

        if (info) {
            info->image->drawSubRect(SCALED(this->x + x), SCALED(this->y + y),
                                     SCALED(subimage->x) / info->prescale,
                                     SCALED(subimage->y) / info->prescale,
                                     SCALED(subimage->width) / info->prescale,
                                     SCALED(subimage->height) / info->prescale);
            return;
        }
    }
    errorFatal("ERROR 1005: Unable to load the image \"%s\".\t\n\nIs %s installed?\n\nVisit the XU4 website for additional information.\n\thttp://xu4.sourceforge.net/", imageName.c_str(), settings.game.c_str());
}

} // End of namespace Ultima4
} // End of namespace Ultima