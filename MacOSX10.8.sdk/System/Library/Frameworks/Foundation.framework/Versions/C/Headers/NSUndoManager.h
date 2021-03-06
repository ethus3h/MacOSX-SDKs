/*	NSUndoManager.h
	Copyright (c) 1995-2012, Apple Inc. All rights reserved.
*/


//
// NSUndoManager is a general-purpose undo stack where clients can register
// callbacks to be invoked should an undo be requested.
//

#import <Foundation/NSObject.h>
#include <stdint.h>

@class NSArray;
@class NSString;

// used with NSRunLoop's performSelector:target:argument:order:modes:
enum {
    NSUndoCloseGroupingRunLoopOrdering		= 350000
};

NS_CLASS_AVAILABLE(10_0, 3_0)
@interface NSUndoManager : NSObject {
    @private
    id 			_undoStack;
    id 			_redoStack;
    NSArray 		*_runLoopModes;
    uint64_t		_NSUndoManagerPrivate1;
    id 			_target;
    id			_proxy;
    void		*_NSUndoManagerPrivate2;
    void		*_NSUndoManagerPrivate3;
}

        /* Begin/End Grouping */

- (void)beginUndoGrouping;
- (void)endUndoGrouping;
    // These nest.

- (NSInteger)groupingLevel;
    // Zero means no open group.

        /* Enable/Disable registration */

- (void)disableUndoRegistration;
- (void)enableUndoRegistration;
- (BOOL)isUndoRegistrationEnabled;

        /* Groups By Event */

- (BOOL)groupsByEvent;
- (void)setGroupsByEvent:(BOOL)groupsByEvent;
    // If groupsByEvent is enabled, the undoManager automatically groups
    // all undos registered during a single NSRunLoop event together in
    // a single top-level group. This featured is enabled by default.

        /* Undo levels */

- (void)setLevelsOfUndo:(NSUInteger)levels;
- (NSUInteger)levelsOfUndo;
    // Sets the number of complete groups (not operations) that should
    // be kept my the manager.  When limit is reached, oldest undos are
    // thrown away.  0 means no limit !

        /* Run Loop Modes */

- (void)setRunLoopModes:(NSArray *)runLoopModes;
- (NSArray *)runLoopModes;

        /* Undo/Redo */

- (void)undo;
    // Undo until a matching begin. It terminates a top level undo if
    // necesary. Useful for undoing when groupByEvents is on (default is
    // on)
- (void)redo;
    // Will redo last top-level undo.

- (void)undoNestedGroup;
    // Undoes a nested grouping without first trying to close a top level
    // undo group.

- (BOOL)canUndo;
- (BOOL)canRedo;
    // returns whether or not the UndoManager has anything to undo or redo

- (BOOL)isUndoing;
- (BOOL)isRedoing;
    // returns whether or not the undo manager is currently in the process
    // of invoking undo or redo operations.

        /* remove */

- (void)removeAllActions;

- (void)removeAllActionsWithTarget:(id)target;

        /* Object based Undo */

- (void)registerUndoWithTarget:(id)target selector:(SEL)selector object:(id)anObject;

        /* Invocation based undo */

- (id)prepareWithInvocationTarget:(id)target;
   // called as:
   // [[undoManager prepareWithInvocationTarget:self] setFont:oldFont color:oldColor]
   // When undo is called, the specified target will be called with
   // [target setFont:oldFont color:oldColor]

- (void)setActionIsDiscardable:(BOOL)discardable NS_AVAILABLE(10_7, 5_0);
   // Set the latest undo action to discardable if it may be safely discarded when a document can not be saved for any reason. An example might be an undo action that changes the viewable area of a document. To find out if an undo group contains only discardable actions, look for the NSUndoManagerGroupIsDiscardableKey in the userInfo dictionary of the NSUndoManagerDidCloseUndoGroupNotification.

// This key is set on the user info dictionary of the NSUndoManagerDidCloseUndoGroupNotification, with a NSNumber boolean value of YES, if the undo group as a whole is discardable.
FOUNDATION_EXPORT NSString * const NSUndoManagerGroupIsDiscardableKey NS_AVAILABLE(10_7, 5_0);

- (BOOL)undoActionIsDiscardable NS_AVAILABLE(10_7, 5_0);
- (BOOL)redoActionIsDiscardable NS_AVAILABLE(10_7, 5_0);
   // Call to see if the next undo or redo action is discardable.

    	/* Undo/Redo action name */

- (NSString *)undoActionName;
- (NSString *)redoActionName;
    // Call undoActionName or redoActionName to get the name of the next action to be undone or redone.
    // Returns @"" if there is nothing to undo/redo or no action names were registered.

- (void)setActionName:(NSString *)actionName;
    // Call setActionName: to set the name of an action.
    // The actionName parameter can not be nil

    	/* Undo/Redo menu item title */

- (NSString *)undoMenuItemTitle;
- (NSString *)redoMenuItemTitle;
    // Call undoMenuItemTitle or redoMenuItemTitle to get the string for the undo or redo menu item.
    // In English they will return "Undo <action name>"/"Redo <action name>" or "Undo"/"Redo" if there is
    // nothing to undo/redo or no action names were set.

    	/* localization hooks */

- (NSString *)undoMenuTitleForUndoActionName:(NSString *)actionName;
- (NSString *)redoMenuTitleForUndoActionName:(NSString *)actionName;
    // The localization of the pattern is usually done by localizing the string patterns in
    // undo.strings. But undo/redoMenuTitleForUndoActionName can also be overridden if
    // localizing the pattern happens to not be sufficient.

@end

FOUNDATION_EXPORT NSString * const NSUndoManagerCheckpointNotification NS_AVAILABLE(10_0, 3_0);
    // This is called before an undo group is begun or ended so any
    // clients that need to lazily register undos can do so in the
    // correct group.

FOUNDATION_EXPORT NSString * const NSUndoManagerWillUndoChangeNotification NS_AVAILABLE(10_0, 3_0);
FOUNDATION_EXPORT NSString * const NSUndoManagerWillRedoChangeNotification NS_AVAILABLE(10_0, 3_0);

FOUNDATION_EXPORT NSString * const NSUndoManagerDidUndoChangeNotification NS_AVAILABLE(10_0, 3_0);
FOUNDATION_EXPORT NSString * const NSUndoManagerDidRedoChangeNotification NS_AVAILABLE(10_0, 3_0);

FOUNDATION_EXPORT NSString * const NSUndoManagerDidOpenUndoGroupNotification NS_AVAILABLE(10_0, 3_0);
FOUNDATION_EXPORT NSString * const NSUndoManagerWillCloseUndoGroupNotification NS_AVAILABLE(10_0, 3_0);

// This notification is sent after an undo group closes. It should be safe to undo at this time.
FOUNDATION_EXPORT NSString * const NSUndoManagerDidCloseUndoGroupNotification NS_AVAILABLE(10_7, 5_0);

