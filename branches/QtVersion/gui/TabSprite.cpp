/*
  Copyright (C) 2008-2010 Laurent Cozic. All right reserved.
  Use of this source code is governed by a GNU/GPL license that can be
  found in the LICENSE file.
*/

#include <stable.h>

#include <FilePaths.h>
#include <TabSprite.h>
#include <Style.h>

using namespace appetizer;

TabSprite::TabSprite(GraphicsWindow* parentWindow): GraphicsItem(parentWindow) {
  folderItemId_ = -1;

  backgroundSprite_ = new NineSliceItem(this->parentWindow());
  backgroundSprite_->loadBackgroundImage(FilePaths::GetSkinFile("IconOverlayUp.png"));
  backgroundSprite_->setAcceptedMouseButtons(Qt::NoButton);
  addItem(backgroundSprite_);

  textSprite_ = new QGraphicsSimpleTextItem();
  textSprite_->setAcceptedMouseButtons(Qt::NoButton);
  addItem(textSprite_);

  setMaxWidth(100);

  applySkin();
}


void TabSprite::applySkin() {
  textSprite_->setFont(Style::tab.textFormat.font());
  textSprite_->setBrush(QBrush(Style::tab.textFormat.color));

  invalidate();
}


void TabSprite::loadFolderItem(int folderItemId) {
  folderItemId_  = folderItemId;
  invalidate();
}


FolderItem* TabSprite::folderItem() {
  return FolderItem::getFolderItemById(folderItemId_);
}


void TabSprite::updateDisplay() {
  GraphicsItem::updateDisplay();

  FolderItem* folderItem = this->folderItem();

  backgroundSprite_->resize(width(), height());

  textSprite_->setText(folderItem ? folderItem->name() : "");

  QRectF r = textSprite_->boundingRect();
  int y = floor((height() - r.height()) / 2);
  textSprite_->setX(10);
  textSprite_->setY(y);
}